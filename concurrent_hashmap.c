#include "concurrent_hashmap.h"
#include <stdlib.h>
#include <stdio.h>



void create()
{
		
	first_key=(key*)malloc((sizeof(key))*HASHMAP_INIT_SIZE);
	free(first_key);
	entry_count=0;
	keypointer=first_key;
	//values=(value*)malloc(HASHMAP_INIT_SIZE);
	
	printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);
	//first_value=values;
	hashmap_size=HASHMAP_INIT_SIZE;
	printf("Hashmap size: %d\n\n",hashmap_size);
}

hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash);
}
/*int hash(char* key)
{
	return ((int)atoi(*key)+3)%(hashmap_size);
}*/
//Increases the memory allocated for the hashmap
void mem_refresh()
{
	hashmap_size=hashmap_size*2;
	first_key=(key*)realloc(first_key,hashmap_size);
	//values=(value*)realloc(first_value,hashmap_size+mem);
}

void put(char* username, char* ip, char* status)
{
	key new_key;
	value new_value;
	if(entry_count>hashmap_size*0.7)
		mem_refresh();

	
	int found=-1;
	int probes=0;	//Counter of probes to insert the key
	
	
	

	
	//Creates the hashcode for the new entry
	int hashCode=hash(username)%hashmap_size;
	printf("hashCode %d\n",hashCode);
	fflush(stdout);	
	keypointer=keypointer+hashCode*sizeof(key);
	
	while(found==-1)
	{
		
		
		if((*keypointer).keyname==NULL)
		{
		
			found=1;
			
			//Puts the entry in the hashmap
			new_value.user=(char*)malloc(sizeof(*username));
			new_value.ip=(char*)malloc(sizeof(*ip));
			new_value.status=(char*)malloc(sizeof(*status));
			new_value.user=username;
			new_value.ip=ip;
			new_value.status=status;
			//*values=new_value;
			
			new_key.keyname=username;
			
			
			
			new_key.value=(value*)malloc(sizeof(value));
			//printf("VALUE:%d",new_key.value);
			*keypointer=new_key;
			*new_key.value=new_value;
			printf("key:%s\n",(*keypointer).keyname);
			printf("ip:%s\n",(*(*keypointer).value).ip);
			
			printf("keypointer %d\n",keypointer);
			printf("first key %d\n",first_key);
			printf("HASHMAP SIZE: %d\n",hashmap_size);
			entry_count++;
			keypointer=first_key;
			
		}
		else 
		{
		
			probes++;
			//printf("%d",probes);
			keypointer=keypointer+sizeof(key);
			
		}
	}
	//values++;
	puts("Numero di probes effettuati:");
	
	//Stampa il numero di probes effettuati per l'inserimento
	printf("%d\n\n",probes);
	
	keypointer=first_key;
	/*printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);*/
	//snprintf("%d\n",p);
	
	
	//Dinamycally increases the memory allocated for the hasmap
	/*key="ehi";
	key++;
	key="bella";
	key++;
	key=user;*/
}

//Mostra l'hashmap
void show()
{
	keypointer=first_key;
	puts("Hashmap:");
	printf("first key: %d\n",first_key);
	printf("keypointer: %d\n",keypointer);
	//snprintf("%d\n",p);
	while(keypointer<first_key+hashmap_size*sizeof(key))
	{
		if((*keypointer).keyname!=NULL)
		{
			printf("\n\n%s\n",(*keypointer).keyname);
			printf("keypointer:%d\n",keypointer);
			printf("ip:%s\n",(*(*keypointer).value).ip);
			printf("%s\n",(*(*keypointer).value).status);
		}
		//printf("p: %d\n",p);
		keypointer+=sizeof(key);
	}
}

int main()
{
	create();
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
	show();

	
	
}
