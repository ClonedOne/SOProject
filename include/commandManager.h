#ifndef COMMANDMANAGER
#define COMMANDMANAGER

#define CLIENT_COMMAND_LENGTH 1
#define ANSWER_LENGTH 2
#define ACK_FROM_SERVER_LENGTH 1
#define ACK_FROM_SERVER '1'
#define REFUSE_FROM_SERVER_LENGTH 1
#define REFUSE_FROM_SERVER '0'
#define ERROR_FROM_SERVER_LENGTH 1
#define ERROR_FROM_SERVER '0'


int getClientCommand(char* command);
int getAnswer(char* answer);



#endif
