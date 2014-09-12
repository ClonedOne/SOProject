
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "login_parser.h"
#include "commandManager.h"


int getInfoByUser(char* user, char** info)
{
	int fd,found,eof=0;
	FILE *fp;
	
	char* buf = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* readUser = malloc(MAX_USERNAME_LENGTH);
	char* password = malloc(MAX_PASSWORD_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);

	
	fp=fopen(USER_LOGIN_FILE,"r");

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
			//COMPLETATO(?)

			buf=malloc(SEPARATOR_LENGTH);
			if(fgets(buf,SEPARATOR_LENGTH,fp)==NULL)
				return 0;

		
		}

	}
	fclose(fp);
	return found;
	
	
	
	
	
}

int checkUserRegistered(char *user)
{
	int fd,found,eof=0;
	FILE *fp;
	
	char* buf = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* readUser = malloc(MAX_USERNAME_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);

	
	fp=fopen(USER_LOGIN_FILE,"r");

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
			found=1;
		
		
		}
		else
		{
			//COMPLETATO(?)

			buf=malloc(SEPARATOR_LENGTH);
			if(fgets(buf,SEPARATOR_LENGTH,fp)==NULL)
				return 0;

		
		}

	}
	fclose(fp);
	return found;
	
	
	
	
	
}

int checkUserLogin(char* user, char* password)
{
	int found;
	char** info = malloc(sizeof(char*)*2);
	found = getInfoByUser(user,info);

	if(found!=0)	
	{	
		if(strcmp(password,info[0])==0)
			return 1;
		else
			return 0;		
	}

	return 0;

}

int clientRegistration(char* user, char* password)
{
	
	FILE *fp;
	char* newLine="\n";

	//OPEN FILE
	if(fp=fopen(USER_LOGIN_FILE,"a")==-1)
	{
		perror("Errore apertura del file durante la registrazione");
		return -1;	
	}
	
	//WRITE USER
	if(fwrite("user:", USERNAME_HEADER_LENGTH, 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(user, strlen(user), 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(newLine, 1, 1, fp)<1)
	{
		perror("Errore in scrittura newLine nel file durante la registrazione");
		return -1;	
	}

	//WRITE PASSWORD
	if(fwrite("password:", PASSWORD_HEADER_LENGTH, 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(user, strlen(password), 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(newLine, 1, 1, fp)<1)
	{
		perror("Errore in scrittura newLine nel file durante la registrazione");
		return -1;	
	}

	//NEW LINE
	if(fwrite(newLine, 1, 1, fp)<1)
	{
		perror("Errore in scrittura newLine nel file durante la registrazione");
		return -1;	
	}
	
	//CLOSE FILE
	if(fclose(fp)==-1)
	{
		perror("Errore in chusura del file durante la registrazione");
		return -1;	
	}
	
	return 1;
	

	
}





