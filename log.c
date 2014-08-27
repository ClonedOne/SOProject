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
