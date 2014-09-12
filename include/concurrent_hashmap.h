#ifndef CONCURRENT_HASHMAP
#define CONCURRENT_HASHMAP
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

key* hashmap;
key* createHashmap();
long hash(unsigned char *str);
void hashmap_mem_refresh(key* first_key);
void putIntoHashmap(key* first_key, char* username, char* ip, char* status);
value* getFromHashmap(key* first_key, char* given_key);
value* deleteFromHashmap(key* first_key, char* given_key);
void hashmap_show(key* first_key);
value** hashmap_retrieveAvailableUsers(key* first_key);
int hashmap_sem;
int hashmap_size;
#endif








