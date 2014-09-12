#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "concurrent_hashmap.h"


void clearLog()
{
	FILE* fp;
	fp=fopen(LOG_FILE,"w");
}

key* restoreConnectedUsers()
{
	//DA FINIRE	

	int fd,found;
	FILE *fp;
	
	char* user = (char*) malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* status = (char*) malloc(MAX_STATUS_LENGTH);
	char* ip = (char*) malloc(MAX_IP_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);
	key* hashmap=createHashmap();
	
	
	fp=fopen(LOG_FILE,"r");

	do
	{
		user = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
		fgets(user,MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH+2,fp);
		user[strlen(user)-1]='\0';
		user+=USERNAME_HEADER_LENGTH;
	
		status=malloc(MAX_STATUS_LENGTH+STATUS_HEADER_LENGTH+2);
		fgets(status,MAX_STATUS_LENGTH+STATUS_HEADER_LENGTH+2,fp);
		status[strlen(status)-1]='\0';
		status+=STATUS_HEADER_LENGTH;
		
	
		ip=malloc(MAX_IP_LENGTH+IP_HEADER_LENGTH+2);
		fgets(ip,MAX_IP_LENGTH+IP_HEADER_LENGTH+2,fp);
		ip[strlen(ip)-1]='\0';
		ip+=IP_HEADER_LENGTH;

		putIntoHashmap(hashmap, user, status, ip);
	}
	while(fgets(separator,SEPARATOR_LENGTH,fp)!=NULL);
	return hashmap;
}

int saveInLog(char* user, char* ip, char* status)
{
	
	FILE *fp;
	char* newLine="\n";
	
	//OPEN FILE
	
	fp=fopen(LOG_FILE,"a");

	if(fwrite(newLine, 1, 1, fp)<1)
	{
		perror("Errore in scrittura newLine nel file durante la registrazione");
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


	//WRITE IP
	if(fwrite("ip:", IP_HEADER_LENGTH, 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(ip, strlen(ip), 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(newLine, 1, 1, fp)<1)
	{
		perror("Errore in scrittura newLine nel file durante la registrazione");
		return -1;	
	}


	//WRITE STATUS
	if(fwrite("status:", STATUS_HEADER_LENGTH, 1, fp)<1)
	{
		perror("Errore in scrittura password nel file durante la registrazione");
		return -1;	
	}
	if(fwrite(status, strlen(status), 1, fp)<1)
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

int deleteFromLog(char* userToDelete)
{
	FILE *fp;
	FILE* newfp;
	
	char* user = (char*) malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* status = (char*) malloc(MAX_STATUS_LENGTH);
	char* ip = (char*) malloc(MAX_IP_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);
	char* newLine="\n";
	


	fp=fopen(LOG_FILE,"r");
	newfp=fopen(NEW_LOG_FILE,"w+");

	do
	{
			user = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
			fgets(user,MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH+2,fp);
			user[strlen(user)-1]='\0';
			user+=USERNAME_HEADER_LENGTH;
	
			status=malloc(MAX_STATUS_LENGTH+STATUS_HEADER_LENGTH+2);
			fgets(status,MAX_STATUS_LENGTH+STATUS_HEADER_LENGTH+2,fp);
			status[strlen(status)-1]='\0';
			status+=STATUS_HEADER_LENGTH;
		
	
			ip=malloc(MAX_IP_LENGTH+IP_HEADER_LENGTH+2);
			fgets(ip,MAX_IP_LENGTH+IP_HEADER_LENGTH+2,fp);
			ip[strlen(ip)-1]='\0';
			ip+=IP_HEADER_LENGTH;

		if(strcmp(userToDelete,user)!=0)
		{
			//WRITE USER
			if(fwrite("user:", USERNAME_HEADER_LENGTH, 1, newfp)<1)
			{
				perror("Errore in scrittura password nel file durante la registrazione");
				return -1;	
			}
			if(fwrite(user, strlen(user), 1, newfp)<1)
			{
				perror("Errore in scrittura password nel file durante la registrazione");
				return -1;	
			}
			if(fwrite(newLine, 1, 1, newfp)<1)
			{
				perror("Errore in scrittura newLine nel file durante la registrazione");
				return -1;	
			}


			//WRITE IP
			if(fwrite("ip:", IP_HEADER_LENGTH, 1, newfp)<1)
			{
				perror("Errore in scrittura password nel file durante la registrazione");
				return -1;	
			}
			if(fwrite(ip, strlen(ip), 1, newfp)<1)
			{
				perror("Errore in scrittura password nel file durante la registrazione");
				return -1;	
			}
			if(fwrite(newLine, 1, 1, newfp)<1)
			{
				perror("Errore in scrittura newLine nel file durante la registrazione");
				return -1;	
			}


			//WRITE STATUS
			if(fwrite("status:", STATUS_HEADER_LENGTH, 1, newfp)<1)
			{
				perror("Errore in scrittura password nel file durante la registrazione");
				return -1;	
			}
			if(fwrite(status, strlen(status), 1, newfp)<1)
			{
				perror("Errore in scrittura password nel file durante la registrazione");
				return -1;	
			}
			if(fwrite(newLine, 1, 1, newfp)<1)
			{
				perror("Errore in scrittura newLine nel file durante la registrazione");
				return -1;	
			}


			//NEW LINE
			if(fwrite(newLine, 1, 1, newfp)<1)
			{
				perror("Errore in scrittura newLine nel file durante la registrazione");
				return -1;	
			}
		}
	}
	while(fgets(separator,SEPARATOR_LENGTH,fp)!=NULL);
	fclose(fp);
	fclose(newfp);

	if(unlink (LOG_FILE)==-1)
		puts("Errore eliminazione vecchio file di log");
	else
		puts("Vecchio file di log eliminato con successo");

	rename (NEW_LOG_FILE, LOG_FILE);

	
}
