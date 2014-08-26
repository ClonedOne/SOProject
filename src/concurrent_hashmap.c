#include "concurrent_hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


value* first_value;
key* keypointer;
value* values;
key sentinel_key;
char* sentinel;
int hashmap_size,entry_count,ds_sem;
struct sembuf oper;


key* createHashmap()
{
	key* first_key;
	//Creates semaphore for concurrent access
	ds_sem = semget(SEMAPHORE_KEY,SEMAPHORE_SIZE,IPC_CREAT|IPC_EXCL|0666);
	if(ds_sem==-1)
	{
		puts("Fallimento durante la creazione del semaforo");
		exit(-1);
	}	
	first_key=(key*)malloc(sizeof(key)*HASHMAP_INIT_SIZE);
	//Initialize the sentinel key for delection of old entries
	sentinel=(char*)malloc(strlen("SENTINEL")+1);
	*sentinel="SENTINEL";
	
	
	//free(first_key);
	entry_count=0;
	keypointer=first_key;
	//values=(value*)malloc(HASHMAP_INIT_SIZE);
	
	#ifdef DEBUG
	printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);
	//first_value=values;
	#endif
	hashmap_size=HASHMAP_INIT_SIZE;
	#ifdef DEBUG
	printf("Hashmap size: %d\n\n",hashmap_size);
	#endif
	return first_key;
}

long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash);
}


//Increases the memory allocated for the hashmap
void hashmap_mem_refresh(key* first_key)
{
	hashmap_size=hashmap_size*2;
	first_key=(key*)realloc(first_key,hashmap_size*sizeof(key));
}

void putIntoHashmap(key* first_key, char* username, char* ip, char* status)
{
	int sem_check;
	oper.sem_op=0;
	oper.sem_num=0;
	oper.sem_flg=0;
	sem_check=semop(ds_sem,&oper,1);
	if(sem_check==-1)
	{
		puts("Errore durante l'operazione di sincronizzazione sul semaforo");
		exit(-1);
	}
	key* new_key=malloc(sizeof(key));
	if(entry_count>hashmap_size*0.7)
		hashmap_mem_refresh(first_key);

	
	int found=-1;
	int probes=0;	//Counter of probes to insert the key
	
	
	

	
	//Creates the hashcode for the new entry
	int hashCode=hash(username)%hashmap_size;
	#ifdef DEBUG	
	printf("hashCode %d\n",hashCode);
	#endif
	keypointer=keypointer+hashCode;
	
	while(found==-1)
	{
		
		
		if((*keypointer).keyname==NULL || keypointer->keyname==sentinel)
		{
		
			found=1;
		
			//Create the new entry 
			new_key->value=(value*)malloc(sizeof(value));
			new_key->value->user=(char*)malloc(strlen(username)+1);
			new_key->value->ip=(char*)malloc(strlen(ip)+1);
			new_key->value->status=(char*)malloc(strlen(status)+1);
			new_key->value->user=username;
			new_key->value->ip=ip;
			new_key->value->status=status;
			new_key->keyname=(char*)malloc(strlen(username)+1);
			new_key->keyname=username;
			
			//Once the entry has been created, store it in the hashmap
			*keypointer=*new_key;
			free(new_key);
			#ifdef DEBUG
			printf("key:%s\n",(*keypointer).keyname);
			printf("ip:%s\n",keypointer->value->ip);
			printf("keypointer %d\n",keypointer);
			printf("first key %d\n",first_key);
			printf("HASHMAP SIZE: %d\n",hashmap_size);
			#endif
			entry_count++;
			keypointer=first_key;
		}
		else 
		{
		
			probes++;
			keypointer++;
			
		}
	}
	//values++;
	#ifdef DEBUG
	puts("Numero di probes effettuati:");
	
	//Stampa il numero di probes effettuati per l'inserimento
	printf("%d\n\n",probes);
	#endif
	keypointer=first_key;
	
	oper.sem_op=-1;
	sem_check=semop(ds_sem,&oper,1);
	if(sem_check==-1)
	{
		puts("Errore durante l'operazione di decremento del semaforo");
		exit(-1);
	}
	
}

//Returns a pointer to the value of the given key. Returns NULL if the given key doesn't exist.
value* getFromHashmap(key* first_key, char* given_key)
{
		keypointer=first_key;

		int sem_check;
		oper.sem_op=0;
		oper.sem_num=0;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}
		
	
		
		value* result=(value*)malloc(sizeof(value));
		int found=-1;
		int probes=0;	//Counter of probes to insert the key

	
		//Creates the hashcode for the new entry
		int hashCode=hash(given_key)%hashmap_size;
	
	
		keypointer=keypointer+hashCode;
	
		while(found==-1 && keypointer->keyname!=NULL && probes<hashmap_size)
		{
			
			if(keypointer->keyname!=NULL && keypointer->keyname!=sentinel)
			{
				printf("Cerco %s\n", given_key);
				printf("Trovo %s\n",keypointer->keyname);
				if(strcmp(keypointer->keyname,given_key)==0)
				{
								
					found=1;
					*result=*(keypointer->value);
			
				}
				else 
				{
					probes++;
					//printf("%d",probes);
					keypointer++;
			
				}
			}
			else
			{
				probes++;
				keypointer++;
			}
		}
		puts("Numero di probes effettuati:");
		//Stampa il numero di probes effettuati per il recupero dell'entry
		printf("%d\n\n",probes);
	
		if(found==-1)
			result=NULL;
		keypointer=first_key;
		
		oper.sem_op=0;
		oper.sem_num=-1;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}
		
		
		return result;
}


//Delete the given key in the hashmap (if exist). Returns a pointer to the removed entry, a NULL pointer if it doesn't exist
value* deleteFromHashmap(key* first_key, char* given_key)
{
		keypointer=first_key;
		
		int sem_check;
		oper.sem_op=0;
		oper.sem_num=0;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}
		
		
		value* result=(value*)malloc(sizeof(value));
		int found=-1;
		int probes=0;	//Counter of probes to insert the key
	
	
		//Creates the hashcode for the new entry
		int hashCode=hash(given_key)%hashmap_size;
	
	
		keypointer=keypointer+hashCode;
	
		while(found==-1 && keypointer->keyname!=NULL && probes<hashmap_size)
		{
			
			if(keypointer->keyname!=NULL && keypointer->keyname!=sentinel)
			{
				printf("Cerco %s\n", given_key);
				printf("Trovo %s\n",keypointer->keyname);
				if(strcmp(keypointer->keyname,given_key)==0)
				{
								
					found=1;
					*result=*(keypointer->value);
					keypointer->keyname=sentinel;
			
				}
				else 
				{
					probes++;
					printf("%d",probes);
					keypointer++;
			
				}
			}
			else
			{
				probes++;
				keypointer++;
			}
		}
		#ifdef DEBUG
		puts("Numero di probes effettuati:");
		
		//Stampa il numero di probes effettuati per il recupero dell'entry
		printf("%d\n\n",probes);
		#endif
	
		if(found==-1)
			result=NULL;
		keypointer=first_key;
		
		oper.sem_op=0;
		oper.sem_num=0;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}
		return result;
}


//Mostra l'hashmap
void hashmap_show(key* first_key)
{
	keypointer=first_key;
	
	
	puts("Hashmap:");
	printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);
	
	//snprintf("%d\n",p);
	while(keypointer<first_key+hashmap_size)
	{
		if(keypointer->keyname!=NULL && keypointer->keyname!=sentinel)
		{
			printf("\n\n%s\n",keypointer->keyname);
			printf("keypointer:%d\n",keypointer);
			printf("ip:%s\n",keypointer->value->ip);
			printf("%s\n",keypointer->value->status);
		}
		//printf("p: %d\n",p);
		keypointer++;
	}
}

/*int main()
{
	//create();
	/*put("zio","1.1.1.1","off");
	put("alex","2.2.2.2","off");
	put("nope","2.2.2.2","off");
	put("zeta","2.2.2.2","off");
	put("uncle","2.2.2.2","off");
	put("vega","1.1.1.1","off");
	put("zio","2.2.2.2","off");
	put("vvvvvv","2.2.2.2","on");
	put("asd","2.2.2.2","on");
	put("sev","1.1.1.1","on");
	put("scadente","2.2.2.2","on");
	put("lol","2.2.2.2","on");
	put("zio","2.2.2.2","on");
	put("zio","1.1.1.1","off");
	put("alex","2.2.2.2","off");
	put("nope","2.2.2.2","off");
	put("zeta","2.2.2.2","off");
	put("uncle","2.2.2.2","off");
	put("vega","1.1.1.1","off");
	put("zio","2.2.2.2","off");
	put("vvvvvv","2.2.2.2","on");
	put("asd","2.2.2.2","on");
	put("sev","1.1.1.1","on");
	put("scadente","2.2.2.2","on");
	put("lol","2.2.2.2","on");
	put("zio","2.2.2.2","on");
	put("zio","1.1.1.1","off");
	put("alex","2.2.2.2","off");
	put("nope","2.2.2.2","off");
	put("zeta","2.2.2.2","off");
	put("uncle","2.2.2.2","off");
	put("vega","1.1.1.1","off");
	put("zio","2.2.2.2","off");
	put("vvvvvv","2.2.2.2","on");
	put("asd","2.2.2.2","on");
	put("sev","1.1.1.1","on");
	put("scadente","2.2.2.2","on");
	put("lol","2.2.2.2","on");
	put("zio","2.2.2.2","on");
	put("zio","1.1.1.1","off");
	put("alex","2.2.2.2","off");
	put("nope","2.2.2.2","off");
	put("zeta","2.2.2.2","off");
	put("uncle","2.2.2.2","off");
	put("vega","1.1.1.1","off");
	put("zio"a,"2.2.2.2","off");*/
	//put("vvvvvv","2.2.2.2","on");
	//put("asd","2.2.2.2","on");
	//put("sev","1.1.1.1","on");
	/*put("scadente","2.2.2.2","on");
	put("lol","2.2.2.2","on");
	put("zio","2.2.2.2","on");
	puts("------------TEST FUNZIONE SHOW------------");
	show();
	/*printf("%d\n",sizeof(key));
	printf("%d\n",((char *)(keypointer+1))-((char*)keypointer));*/
	/*puts("------------TEST FUNZIONE GET------------");
	value* firstValue= get("alex");
	value* secondValue= get("zio");
	value* thirdValue= get("nope");
	value* fourthValue= get("false");
	puts("CHIAVE1: alex");
	if(firstValue==NULL)
		puts("NULL");
	else
	{
		puts((*firstValue).user);
		puts((*firstValue).ip);
		puts((*firstValue).status);
	}
	puts("CHIAVE2: zio");
	if(secondValue==NULL)
		puts("NULL");
	else
	{
		puts((*secondValue).user);
		puts((*secondValue).ip);
		puts((*secondValue).status);
	}
	puts("CHIAVE3: nope");
	if(thirdValue==NULL)
		puts("NULL");
	else
	{
		puts((*thirdValue).user);
		puts((*thirdValue).ip);
		puts((*thirdValue).status);
	}
	puts("CHIAVE4: false");
	if(fourthValue==NULL)
		puts("NULL");
	else
	{
		puts((*fourthValue).user);
		puts((*fourthValue).ip);
		puts((*fourthValue).status);
	}
	
	puts("------------TEST FUNZIONE DELETE------------");
	puts("Cancello l'entry con chiave asd");
	value* a=delete("asd");
	if(a!=NULL)
	{
		puts("Entry cancellata:\n");
		puts(a->user);
		puts(a->ip);
		puts(a->status);
	}
	else
	puts("Entry non presente nell'hashmap!");	
	puts("Cancello l'entry con chiave asd");
	a=delete("asd");
	if(a!=NULL)
	{
		puts("Entry cancellata:\n");
		puts(a->user);
		puts(a->ip);
		puts(a->status);
	}
	else
	puts("Entry non presente nell'hashmap!");
	a=get("asd");
	if(a!=NULL)
	{
		puts("Entry trovata:\n");
		puts(a->user);
		puts(a->ip);
		puts(a->status);
	}
	else
	puts("Entry non presente nell'hashmap!");
	free(first_key);

	
	
}*/
