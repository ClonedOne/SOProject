
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "login_parser.h"


int getInfoByUser(char* user, char** info)
{
	int fd,found,eof=0;
	FILE *fp;
	
	char* buf = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* readUser = malloc(MAX_USERNAME_LENGTH);
	char* password = malloc(MAX_PASSWORD_LENGTH);
	char* ip = malloc(MAX_IP_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);

	
	fp=fopen(FILENAME,"r");

	found=0;
	eof=0;
	while(found!=1)
	{

		fgets(buf,MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH+2,fp);
		buf[strlen(buf)-1]='\0';
		buf+=USERNAME_HEADER_LENGTH;
	
	
		strcpy(readUser,buf);
	
	
		if(strcmp(readUser,user)==0)
		{
			buf=malloc(MAX_PASSWORD_LENGTH+PASSWORD_HEADER_LENGTH+2);
			fgets(buf,MAX_PASSWORD_LENGTH+PASSWORD_HEADER_LENGTH+2,fp);
			buf[strlen(buf)-1]='\0';
		
			buf+=PASSWORD_HEADER_LENGTH;
			strcpy(password,buf);
			info[0]=password;

			found=1;
		
		
		}
		else
		{
			//DA COMPLETARE SCORRERE ALTRE RIGHE DEL FILE

			buf=malloc(SEPARATOR_LENGTH);
			if(fgets(buf,SEPARATOR_LENGTH,fp)==NULL)
				return 0;

		
		}

	}
	
	
	
	
	
}



