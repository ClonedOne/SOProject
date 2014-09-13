#include "commandManager.h"

int getClientFirstCommand(char* command)
{
	if(strlen(command)==CLIENT_COMMAND_LENGTH) 
	{
		if(command[0]=='1')
			return 1;
		else if(command[0]=='0') 
			return 0;
	}
	return -1;
}

int getClientSecondCommand(char* command)
{
	if(strlen(command)==CLIENT_COMMAND_LENGTH) 
	{
		if(command[0]=='2' || command[0]=='3' || command[0]=='4' || command[0]=='5')
			return 1;
			
	}
	return -1;
}

int getAnswer(char* answer)
{
	if(strlen(answer)==ANSWER_LENGTH) 
	{
		if(answer[0]=='Y')
			return 1;
		else if(answer[0]=='N') 
			return 0;
	}
	return -1;
}
