#ifndef CONCURRENT_HASHMAP
#define HASHMAP
#define HASHMAP_INIT_SIZE 10
#define INCREASE_MEMORY 1
#define SEMAPHORE_KEY 30
#define SEMAPHORE_SIZE 1




typedef struct 
{
	char* user;
	char* ip;
	char* status;
} value;

typedef struct 
{
	char* keyname;
	value* value;
} key;

void createHashmap();
long hash(unsigned char *str);
void hashmap_mem_refresh();
void putIntoHashmap(char* username, char* ip, char* status);
value* getFromHashmap(char* given_key);
value* deleteFromHashmap(char* given_key);
void hashmap_show();
#endif








