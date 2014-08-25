
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
#define SEPARATOR_LENGTH 2
#define FILENAME "usersInfo.txt"


int getInfoByUser(char* user, char** info)
{
	int fd,found;
	FILE *fp;
	
	char* buf = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* readUser = malloc(MAX_USERNAME_LENGTH);
	char* status = malloc(MAX_STATUS_LENGTH);
	char* ip = malloc(MAX_IP_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);

	
	fp=fopen(FILENAME,"r");

	found=0;
	
	while(found!=1)
	{

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

			found=1;
		
		
		}
		else
		{
			//DA COMPLETARE SCORRERE ALTRE RIGHE DEL FILE

			buf=malloc(SEPARATOR_LENGTH);
			if(fgets(buf,SEPARATOR_LENGTH,fp)==NULL)
				return 1;

		
		}
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


