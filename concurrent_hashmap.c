#include "concurrent_hashmap.h"
#include <stdlib.h>
#include <stdio.h>


void create()
{
	first_key=(key*)malloc((sizeof(key))*HASHMAP_INIT_SIZE);
	free(first_key);
	keypointer=first_key;
	entry_count=0;
	//values=(value*)malloc(HASHMAP_INIT_SIZE);
	
	printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);
	//first_value=values;
	hashmap_size=HASHMAP_INIT_SIZE;
	printf("Hashmap size: %d\n\n",hashmap_size);
}
int hash(char* key)
{
	return ((int)(*key+3))%(hashmap_size);
}
//Increases the memory allocated for the hashmap
void mem_refresh()
{
	hashmap_size=hashmap_size*2;
	//keypointer=first_key;
	first_key=(key*)realloc(first_key,hashmap_size);
	//free(keypointer);
	keypointer=first_key;
	//values=(value*)realloc(first_value,hashmap_size+mem);
}

void put(char* username, char* ip, char* status)
{

	key new_key;
	value new_value;
	int found=-1;
	int probes=0;	//Counter of probes to insert the key
	
	
	

	
	//Creates the hashcode for the new entry
	int hashCode=hash(username);
	printf("hashCode %d\n",hashCode);
	fflush(stdout);	//prodotti scadenti
	keypointer=keypointer+(hashCode*sizeof(key));
	
	//Dinamycally checks the memory allocated for the hasmap
	//if(entry_count>hashmap_size*0.7)
	//	mem_refresh();
			
	while(found==-1)
	{
			
		if((*keypointer).keyname==NULL)
		{
		
			found=1;

			new_value.user=username;
			new_value.ip=ip;
			new_value.status=status;
			
			new_key.keyname=username;
			new_key.value=(value*)malloc(sizeof(new_value));
			*new_key.value=new_value;
			*keypointer=new_key;
			printf("new_key %s\n",(*keypointer).keyname);
			printf("keypointer %d\n",keypointer);
			printf("first key %d\n",first_key);
			keypointer=first_key;
			entry_count++;
		}
		else 
		{
			probes++;
			//keypointer=(key*)(((unsigned int)(keypointer+sizeof(key)))%(hashmap_size*sizeof(key)));
			/*unsigned long offset = keypointer+sizeof(key);
			unsigned long  mod_offset= offset%(hashmap_size*sizeof(key));
			
			keypointer=keypointer+mod_offset;
			//(printf("KEYPOINTER AFTER PROBE : %d\n\n",keypointer);
			fflush(stdout);*/
			if(keypointer >= first_key+ hashmap_size*sizeof(key))
				keypointer = first_key;
			else
				keypointer += sizeof(key);
		}
	}
	printf("HASHMAP SIZE: %d\n",hashmap_size);
	//values++;
	puts("Numero di probes effettuati:");
	
	//Stampa il numero di probes effettuati per l'inserimento
	printf("%d\n\n",probes);
	
		
	
	
	
}

//Mostra l'hashmap
void show()
{
	key* p = first_key;
	puts("Hashmap:");
	printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);
	printf("lastKey: %d\n", first_key+hashmap_size*sizeof(key));
	
	//snprintf("%d\n",p);
	while(p < first_key+hashmap_size*sizeof(key)-sizeof(key))
	{
		int count = 1;
		//puts("STOSTAMPANDOOOOOOOOOOOO");
		//printf("pirla:: %d\n",p);
		if((*p).keyname!=NULL)
		{
			/*printf("\n\n%s\n",(*((*p).value)).user);
			printf("%s\n",(*((*p).value)).ip);
			printf("%s\n",(*((*p).value)).status);*/
			//printf("%s\n",(*p).keyname);
			//printf("keypointer: %d\n",p);
			puts("ciao");
		}
		p+=sizeof(key);
	}
	printf("pirla:: %d\n",p);
}

int main()
{
	
	create();
	put("malemale","1.1.1.1","on");
	put("malemale","1.1.1.1","on");
	put("malemale","1.1.1.1","on");
	put("malemale","1.1.1.1","on");
	put("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA","1.1.1.1","on");
	put("nonevero","3.3.3.3","off");
	put("AL","2.2.2.2","on");
	//put("uncle"2.2.2.2","on");
	show();
  
	
	
}
