#ifndef LOGINPARSER
#define LOGINPARSER

int getInfoByUser(char* user, char** info);

#define MAX_USERNAME_LENGTH 128 
#define MAX_PASSWORD_LENGTH 128
#define MAX_IP_LENGTH 15
#define USERNAME_HEADER_LENGTH 5 
#define PASSWORD_HEADER_LENGTH 9
#define SEPARATOR_LENGTH 2
#define FILENAME "usersLogin.txt"
#endif 
