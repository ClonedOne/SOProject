#include "log.h"


void clearLog()
{
	FILE* fp;
	fp=fopen(LOG_FILE,"w");
}

restoreConnectedUsers()
{
	//DA FINIRE	

	int fd,found;
	FILE *fp;
	
	char* buf = malloc(MAX_USERNAME_LENGTH+USERNAME_HEADER_LENGTH);
	char* readUser = malloc(MAX_USERNAME_LENGTH);
	char* status = malloc(MAX_STATUS_LENGTH);
	char* ip = malloc(MAX_IP_LENGTH);
	char* separator = malloc(SEPARATOR_LENGTH);

	
	fp=fopen(FILENAME,"r");

	found=0;
	
	while(fgets(buf,SEPARATOR_LENGTH,fp)==NULL)
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

			buf=malloc(SEPARATOR_LENGTH);
			if(fgets(buf,SEPARATOR_LENGTH,fp)==NULL)
				return 1;

		
		}
	}
}