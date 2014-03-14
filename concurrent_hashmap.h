#define HASHMAP_INIT_SIZE 1000


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


key* first_key;
key* keypointer;
int hashmap_size;
int entry_count;



