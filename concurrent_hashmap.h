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








