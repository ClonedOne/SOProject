#ifndef LOGINPARSER
#define LOGINPARSER

int getInfoByUser(char* user, char** info);
int checkUserLogin(char* user, char* password);
int checkUserRegistered(char *user);
int clientRegistration(char* user, char* password);

#define MAX_USERNAME_LENGTH 128 
#define MAX_PASSWORD_LENGTH 128
#define MAX_IP_LENGTH 15
#define USERNAME_HEADER_LENGTH 5 
#define PASSWORD_HEADER_LENGTH 9
#define SEPARATOR_LENGTH 2
#define LOG_FILE "UsersInfo.txt"
#define USER_LOGIN_FILE "usersLogin.txt"
#endif 
