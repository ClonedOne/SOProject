#define HASHMAP_INIT_SIZE 10
#define INCREASE_MEMORY 1



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

/*typedef struct
{
	value* value;
} keytovalue;*/


key* first_key;
value* first_value;
key* keypointer;
value* values;
int hashmap_size;
int entry_count;




