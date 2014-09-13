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
#include "commandManager.h"
#include "auth_service.h"
#include "serv1.h"





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
	server.sin_port = htons(SERVICE_PORT);
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



int clientConnection(int* cli_sock)
{

	int r_size;
	int control;
	char* firstCommand;
	int firstComm,secondComm;

	char* user = malloc(MAX_USERNAME_LENGTH);
	char* password = malloc(MAX_PASSWORD_LENGTH);

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
		puts("Waiting for client first command");
		read(cli_sock, firstCommand, CLIENT_COMMAND_LENGTH);
		
		#ifdef DEBUG
		puts("Comando preso in input:");
		puts(firstCommand);
		#endif

	} while (getClientFirstCommand(firstCommand)==-1);


	
	if(firstComm=='0')
	{
		//Client Registration
		if(registrationRequest(cli_sock)==-1)
			puts("Registrazione al servizio fallita");
		else
			puts("Registrazione al servizio avvenuta con successo");

		
	}
	else if(firstComm=='1')
	{
		//Client login

		if(loginRequest(cli_sock)==-1)
			puts("Login al servizio falita");
		else
			puts("Login al servizio avvenuta con successo");
	}

	
	do{
		puts("Waiting for client second command");
		read(cli_sock, secondComm, CLIENT_COMMAND_LENGTH);
		
		#ifdef DEBUG
		puts("Comando preso in input:");
		puts(secondComm);
		#endif

	} while (getClientSecondCommand(secondComm)==-1);

	if(secondComm=='2')
	{
		//Client Logout
		if(logoutRequest(cli_sock)==-1)
			puts("Logout al servizio fallito");
		else
			puts("Logout al servizio avvenuta con successo");
		
		//Terminates thread

		pthread_exit(-1);

		
	}
	else if(secondComm=='3')
	{
		//Client status 'available'

		if(availableRequest(cli_sock)==-1)
			puts("Aggiornamento di statocdisponibile fallito");
		else
			puts("Aggiornamento di stato disponibile avvenuto con successo");
	}
	else if(secondComm=='4')
	{
		//Client status 'busy'

		if(busyRequest(cli_sock)==-1)
			puts("Aggiornamento di statocdisponibile fallito");
		else
			puts("Aggiornamento di stato occupato avvenuto con successo");
	}
	else if(secondComm=='5')
	{
		//Retrieve available user list

		if(retrieveListRequest(cli_sock)==-1)
			puts("Richiesta lista utenti disponibili al servizio fallita");
		else
			puts("Richeiesta lista utenti disponibili al servizio avvenuta con successo");
	}
	
	
	

	pthread_exit(1);
	
}
















