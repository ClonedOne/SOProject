#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/sem.h>
#include "login_parser.h"
#include "concurrent_hashmap.h"
#include "log.h"


#define CODA 5
#define DIM 1024
#define MAX_PENDING_CONNECTIONS 100
#define ANSWER_LENGTH 2
#define CLIENT_COMMAND_LENGTH 2
#define LOG_FILE "UsersInfo.txt"



int clientConnection(int* cli_sock)
{

	int r_size;
	int control;
	char* user;
	char* password;
	char* firstCommand;
	int firstComm;

	user = malloc(MAX_USERNAME_LENGTH);
	password = malloc(MAX_PASSWORD_LENGTH);

	firstCommand=malloc(ANSWER_LENGTH);	

	//Client should login

	/*do {
		if ((r_size = read(cli_sock, user, MAX_USERNAME_LENGTH)) == -1){
			perror("Read error");
		}

		if ((r_size = read(cli_sock, password, MAX_PASSWORD_LENGTH)) == -1){
			perror("Read error");
		}


		/*if ((control = write(cli_sock, buff, r_size)) == -1){
			perror("Write error");
		}

	}while (r_size > 0);

	if(checkUserLogin(user,password)==1)
		puts("User Logged");
	else
		puts("Bad Login");*/

	
	//Si mette in attesa del comando da parte del client

	do{
		puts("Waiting for client command");
		fgets(firstCommand, CLIENT_COMMAND_LENGTH, stdin);
		
		#ifdef DEBUG
		puts("Comando preso in input:");
		puts(firstCommand);
		#endif

	} while (firstComm = getFirstCommand(firstCommand)==-1);


	
	if(firstComm==1)
	{
		//Client Login
	}
	else
	{
		//Client registration
	}
	

	pthread_exit(0);
	
}

int getFirstCommand(char* command)
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

void main() {


	int serv_sock;
	int cli_sock;
	int rval;
	int length;
	int control;
	int src_file;
	int r_size;
	
	char* buff;
	char f_name[DIM];
	char* answer;	
	int ans;
	pthread_t* tid;
	struct sockaddr_in server;
	struct sockaddr client;
	
	puts("Welcome to Such Chatz server!");
	

	answer=malloc(ANSWER_LENGTH);
	do{
		puts("Do you want to restore connected Users from log file?");
		fgets(answer, ANSWER_LENGTH, stdin);
		
		#ifdef DEBUG
		puts("Risposta presa in input:");
		puts(answer);
		#endif

	} while (ans = getAnswer(answer)==-1);
	answer[strlen(answer) -1] = '\0';

	
	//Inizializzo il semaforo per l'accesso all'hashmap

	hashmap_sem = semget(SEMAPHORE_KEY,SEMAPHORE_SIZE,IPC_CREAT|IPC_EXCL|0666);
	if(hashmap_sem==-1)
	{
		puts("Fallimento durante la creazione del semaforo");
		exit(-1);
	}

	//Creo l'hashmap

	hashmap=createHashmap();	
	
	//Compie l'azione corrispondente alla scelta del sistemista.

	if(ans==1)
	{
		restoreConnectedUsers(hashmap);
	}
	else
	{
		clearLog();	
	}

//open the server socket to accept connections
	if ((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		perror("Could not Open socket");
		exit(EXIT_FAILURE);
	}
	puts("We created the new socket");

//fills in the sockaddr struct 
	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr = INADDR_ANY;

//binds the socket with the sockaddr_in struct previously filled
	if (bind(serv_sock, (struct sockaddr*)&server, sizeof(server)) == -1){
		perror("Could not Bind socket");
		exit(EXIT_FAILURE);
	}
	puts("The binding has been done");

//allows the process to listen for connections
	if(listen(serv_sock, 5) == -1){
		perror("Could not Listen on socket");
		exit(EXIT_FAILURE);
	}
	puts("The process is now listening");

/*
//asks the user for the list file name and opens it
	while(control != 1){
		puts("Please insert the name of the list file");
		fgets(f_name, DIM, stdin);
		int slen = strlen(f_name);
		f_name[slen-1] = '\0';
		if ((src_file = open(f_name, O_RDONLY)) == -1) {
			printf("Couldn not open file\n");
		}else
			control = 1;
	}
	
	length = sizeof(client);*/
	
//accept incoming connections

tid=(pthread_t*)malloc(MAX_PENDING_CONNECTIONS*sizeof(pthread_t));
int i=0;

while(1)
{
	
	while ((cli_sock = accept(serv_sock, &client, & length)) == -1);

	pthread_create(&(tid[i]),NULL,(void*)&clientConnection,(void *) &cli_sock);
	i++;
}	



//Wait user's login
	
	
	
	
	
	
}

/*Istruzioni semaforo 
CREAZIONE:



CODICE


PUT:

int sem_check;
	oper.sem_op=0;
	oper.sem_num=0;
	oper.sem_flg=0;
	sem_check=semop(ds_sem,&oper,1);
	if(sem_check==-1)
	{
		puts("Errore durante l'operazione di sincronizzazione sul semaforo");
		exit(-1);
	}

CODICE:

oper.sem_op=-1;
	sem_check=semop(ds_sem,&oper,1);
	if(sem_check==-1)
	{
		puts("Errore durante l'operazione di decremento del semaforo");
		exit(-1);
	}



GET:

int sem_check;
		oper.sem_op=0;
		oper.sem_num=0;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}


CODICE

oper.sem_op=0;
		oper.sem_num=-1;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}

DELETE:

int sem_check;
		oper.sem_op=0;
		oper.sem_num=0;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}

CODICE

oper.sem_op=0;
		oper.sem_num=0;
		oper.sem_flg=0;
		sem_check=semop(ds_sem,&oper,1);
		if(sem_check==-1)
		{
			puts("Errore durante l'operazione di sincronizzazione sul semaforo");
			exit(-1);
		}

*/
