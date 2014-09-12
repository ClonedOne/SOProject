#ifndef LOG
#define LOG
#ifndef CONCURRENT_HASHMAP
#include "concurrent_hashmap.h"
#endif
#define LOG_FILE "UsersInfo.txt"
#define NEW_LOG_FILE "NewUsersInfo.txt"
#define MAX_USERNAME_LENGTH 128 
#define MAX_STATUS_LENGTH 9
#define MAX_IP_LENGTH 15
#define USERNAME_HEADER_LENGTH 5 
#define STATUS_HEADER_LENGTH 7
#define IP_HEADER_LENGTH 3
#define SEPARATOR_LENGTH 2
void clearLog();
key* restoreConnectedUsers();
int saveInLog(char* user, char* ip, char* status);
#endif

