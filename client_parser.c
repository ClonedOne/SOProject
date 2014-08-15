
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERNAME_LENGTH 128 
#define MAX_STATUS_LENGTH 9
#define MAX_IP_LENGTH 15
#define USERNAME_HEADER_LENGTH 5 
#define STATUS_HEADER_LENGTH 7
#define IP_HEADER_LENGTH 3
#define FILENAME "usersInfo.txt"


int getInfoByUser(char* user, char** info)
{
	int fd;
	FILE *fp;
	
	char* buf = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* readUser = malloc(MAX_USERNAME_LENGTH);
	char* status = malloc(MAX_STATUS_LENGTH);
	char* ip = malloc(MAX_IP_LENGTH);

	
	fp=fopen(FILENAME,"r");
	fgets(buf,MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH+2,fp);
	buf[strlen(buf)-1]='\0';
	buf+=USERNAME_HEADER_LENGTH;
	
	
	strcpy(readUser,buf);
	
	
	if(strcmp(readUser,user)==0)
	{
		buf=malloc(MAX_STATUS_LENGTH+STATUS_HEADER_LENGTH+2);
		fgets(buf,MAX_STATUS_LENGTH+STATUS_HEADER_LENGTH+2,fp);
		buf[strlen(buf)-1]='\0';
		
		buf+=STATUS_HEADER_LENGTH;
		strcpy(status,buf);
		info[0]=status;
		
		buf=malloc(MAX_IP_LENGTH+IP_HEADER_LENGTH+2);
		fgets(buf,MAX_IP_LENGTH+IP_HEADER_LENGTH+2,fp);
		buf[strlen(buf)-1]='\0';
		buf+=IP_HEADER_LENGTH;
		strcpy(ip,buf);
		info[1]=ip;
		
		return 1;
		
		
	}
	else
	{
		//DA COMPLETARE SCORRERE ALTRE RIGHE DEL FILE
		
		info[0]="cacca";
		info[1]="zero";
		return 0;
	}
	
	
	
	
	
	
}

int main()
{
	int found;
	char** info = malloc(sizeof(char*)*2);
	found = getInfoByUser("uncleman11",info);
	if(found!=0)
	{
		puts(info[0]);
		puts(info[1]);
	}
	
}


