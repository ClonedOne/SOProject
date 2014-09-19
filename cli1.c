/* This is a client program. U DONT SAY?!
It consists of 2 threads
	-The first waits for connections from other programs just like this and eventually start a chat with it
	-The second tries to talk to a server and get the address of another program just like this form a list provided by the server itself
The goal of all of it is just to create a sinchronous communication channel between two users over the net.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include "cli1.h"


#define DEBUG


struct winsize ts;
char* callingIP; 			//IP address of the caller
char callingID[IDLEN]; 		//User Name of the caller
char userName[USERNAME]; 	//My User Name
char userPwd[PASSWORD]; 	//My Password
char serverCom[SERV_COM];	//Buffer used to communicate with server
int row_count; 				//number of rows of the terminal window
int inchat; 				//flag that shows that client is currently chatting
int com_res; 				//store the result of the command function
int r;
sem_t sem1;					//semaphore used by threadMain to start threadServer
sem_t sem2;					//sempahore used by threadServer to give control back to threadMain
pthread_t t1, t2, t3; 		//thread handles



//This thread connects to the server and dwonload the clients list
void* func_t_2 (){

	int success;
	int sock;
	char buff[32];
	struct sockaddr_in client;
	
	
//Creates the new socket to communicate with server
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		perror("Could not Open socket");
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	puts("We are starting the Server communication thread\n");
#endif

//fills out the sockaddr struct
	client.sin_family = AF_INET;
	client.sin_port = htons(5000);
	if (inet_aton("192.168.43.107", &client.sin_addr) == 0) {
		perror("Address to network conversion error");
	}
	
//Connects to the server
	if ((connect(sock, (struct sockaddr*) &client, sizeof(client))) == -1){
	perror("couldn't connect");
	}
	
#ifdef DEBUG
	puts("Thread connected\n");
#endif

//Wait for the semaphore
	while(1){
		success = 0;
		sem_wait(&sem1); 
		serverCom[1] = '\0';
		puts("sono qui");
		char letter = serverCom[0];
		switch(letter){
			case '0':
				while(success == 0){
					send(sock, serverCom, SERV_COM,0);
					//puts("ho scritto!!!");
					memset(serverCom, 0, SERV_COM);
					read(sock, serverCom, SERV_COM);
					printf("received ack: %c\n", serverCom[0]);
					if (serverCom[0] == '0')
						puts("Server Operation FAILED");
					//Send User Name to Server			
					do {
						do{
							memset(userName, 0, USERNAME);
							puts("Please insert your User Name, it must be shorter than 32 chars");
							fgets(userName, USERNAME, stdin);
						} while (acceptableString(userName) == 0 || strlen(userName) > USERNAME -1);
						userName[strlen(userName) -1] = '\0';
						send(sock, userName, USERNAME,0);
						memset(serverCom, 0, SERV_COM);
						read(sock, serverCom, SERV_COM);
						if (serverCom[0] == '0')
							puts("User Name not available please insert another");
					}while(serverCom[0] != '1');
					//Send Password to Server
					do{
						userPwd[0] = '\0';
						puts("Please insert your Password, it must be shorter than 32 chars");
						fgets(userPwd, PASSWORD, stdin);
					} while (acceptableString(userPwd) == 0 || strlen(userPwd) > PASSWORD -1);
					userPwd[strlen(userPwd) -1] = '\0';
					write(sock, userPwd, PASSWORD);
					serverCom[0] = '\0';
					read(sock, serverCom, SERV_COM);
					if (serverCom[0] == '0')
						puts("Unfortunately there was a problem with the procedure");
					else 
						success = 1;
				}
			break;
					
			case '1':
				while(success == 0){
					write(sock, serverCom, SERV_COM);
					memset(serverCom, 0, SERV_COM);
					read(sock, serverCom, SERV_COM);
					if (serverCom[0] == '0')
						puts("Server Operation FAILED");
					//Send User Name to Server			
					do {
						do{
							userName[0] = '\0';
							puts("Please insert your User Name");
							fgets(userName, USERNAME, stdin);
						} while (acceptableString(userName) == 0 || strlen(userName) > USERNAME -1);
						userName[strlen(userName) -1] = '\0';		
						write(sock, userName, USERNAME);
						memset(serverCom, 0, SERV_COM);
						read(sock, serverCom, SERV_COM);
						if (serverCom[0] == '0'){
							puts("Server Error");
						}
					//Send Password to Server
						do{
							userPwd[0] = '\0';
							puts("Please insert your Password");
							fgets(userPwd, PASSWORD, stdin);
						} while (acceptableString(userPwd) == 0 || strlen(userPwd) > PASSWORD -1);
						userPwd[strlen(userPwd) -1] = '\0';		
						write(sock, userPwd, PASSWORD);
						memset(serverCom, 0, SERV_COM);
						read(sock, serverCom, SERV_COM);
						if (serverCom[0] == '0')
							puts("Wrong Username and Password combination please insert another");
					}while(serverCom[0] != '1');
					success = 1;
				}
			break;
			
			case '2':
				while (success == 0) {
					serverCom[0] = '2';
					write(sock, serverCom, SERV_COM);
					serverCom[0] = '\0';
					read(sock, serverCom, SERV_COM);
					if (serverCom[0] == '1')
						success = 1;
				}
			break;
			
			case '3':
				while (success == 0) {
					serverCom[0] = '3';
					write(sock, serverCom, SERV_COM);
					serverCom[0] = '\0';
					read(sock, serverCom, SERV_COM);
					if (serverCom[0] == '1')
						success = 1;
				}
			
			break;
			
			case '4':		
				while (success == 0) {
					serverCom[0] = '4';
					write(sock, serverCom, SERV_COM);
					serverCom[0] = '\0';
					read(sock, serverCom, SERV_COM);
					if (serverCom[0] == '1')
						success = 1;
				}
			break;
				
			case '5':
				puts("caso 5");
				do{
					serverCom[0] = '5';
					write(sock, serverCom, SERV_COM);
					memset(serverCom, 0, SERV_COM);
					read(sock, serverCom, SERV_COM);
				}while(serverCom[0] != '1');
				int size;
				do {
					memset(buff, 0, 32);
					if((size = recv(sock, buff, 32, 0)) == -1)
						perror("read error");
					if (size > 0)
						printf("%s\n",buff);
				}while(size > 0);
				success = 1;
			break;
		
			default:
				perror("Communication management problem");
		}
		
		serverCom[0] = '\0';
		if (success == 1);
			sem_post(&sem2);
		
	}
	close(sock);
}



//THE MAIN FUNCTION

int main(int argc, char*argv[]){

	int retT2;		//server talking thread return value
	
	
//Variables initialization phase
	com_res = LISTEN;
	inchat = NOTINCHAT;			
	callingIP = malloc(IPLEN);

//this struct is used to store the dimensions (rows/col) of the terminal window
	ioctl(0, TIOCGWINSZ, &ts);
	signal(SIGWINCH, sigwinch_handler);
	row_count = 0;

//Spawn server talking thread and  create communication semaphores
	if (sem_init(&sem1, 0, SEMZERO) == -1 || sem_init(&sem2, 0, SEMZERO) == -1)
		perror("Couldn't create the semaphores");
	retT2 = pthread_create(&t2, NULL, &func_t_2, NULL);
	if (retT2 != 0)
		perror("Couldn't create the thread!");

//clear screen and ask for command
	clearS();
	puts("Welcome! We are now starting the service!");
	printf("Please select which action to perform:\n Write '0' to Sign Up as a new user\n Write '1' to Log In\n");
	fgets(serverCom, SERV_COM, stdin);
	while (serverCom[0] != '1' && serverCom[0] != '0'){
		puts("Invalid command please repeat your selection");
		serverCom[0] = '\0';
		fgets(serverCom, SERV_COM, stdin);
	}
#ifdef DEBUG
	printf("comando = %c\n",serverCom[0]);
#endif
	sem_post(&sem1);
	sem_wait(&sem2);

	//This loop ends when the user input the ::q command
	while(com_res != QUIT){
		puts("SONO NEL MAIN");
		printf("com res = %d\n", com_res);
		if(com_res == LISTEN)
			func_1();
		else if (com_res == CONNECT)
			func_3();
		else {
			serverCom[0] = '5';
			sem_post(&sem1);
			sem_wait(&sem2);
			com_res = LISTEN;
		}
			
	}

	printf("\033[1;31mQuitting....\033[0m\n");
	sleep(2);
	clearS();
	exit(1);
}


//this thread is used to connect to other clients
void func_3() {
	
	
	int sock;
	struct sockaddr_in client;
	char sendBuf[DIM];
	char recvBuf[DIM];
	
	resetBlockInput();
	callingIP[0] = '\0';
	
//Ask the user for the target of the connection
	printf("Please select the user\n");
	
	fgets(callingIP, DIM, stdin);
	puts("WAT?!?!?");
	
	callingIP[strlen(callingIP) - 1] = '\0';
	
	printf("chosen ip is %s\n",callingIP);
	
//creates the socket
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		perror("Could not Open socket");
		exit(EXIT_FAILURE);
	}
	
//fills up socket information
	client.sin_family = AF_INET;
	client.sin_port = htons(4100);
	if (inet_aton(callingIP, &client.sin_addr) == 0) {
		perror("Address to network conversion error");
		exit(EXIT_FAILURE);
	}

//try to connect
	if (connect(sock,(struct sockaddr *)&client,sizeof(client)) == -1){
		perror("Couldn't connect");
		exit(EXIT_FAILURE);
	}
	
//sends userName to the called client and gets its name
	write(sock, userName, DIM);
	read(sock, callingID, DIM);
	
	
	read(sock, recvBuf, DIM);
	if (recvBuf[0] == 0){
		printf("Your call has benn refused\n");
		return;
	}

//change the communicating socket and STDIN to NONBLOCKING mode
	noBlockSocket(sock);
	noBlockInput();

	inchat = INCHAT;
	stampaHeader();
	
			
//Chat loop
	chat(&sock, sendBuf, recvBuf);

}



//check if the string contains unacceptable characters
int acceptableString(char *s){
	int i = 0;
	int len=strlen(s)-1;
	for(i = 0; i < len; i++){
		if(!(s[i] >= 32 && s[i] < 127))
			return 0;
			
	}
	return 1;
}


//Clear Screen
void clearS (){
	printf("\033[2J\033[H\033[A");
}




//Finds out the increment of lines caused by the new message string
//"1" is used to compensate for strings of the exact length of the terminal
int incr (int slen, int nlen){
	int ret, temp;
	temp = slen + nlen - 1;
	if(temp < 0)
		temp = 0;
	ret = (temp/ts.ws_col) + 1;
	return ret;
}


//Saves current cursor postiion
//force the cursor on the current wiriting line 
//prints the received string and increments the current writing line
//restore previous cursor position
void printRecvUp(char* recvBuf, int length) {
	int written_rows;
	written_rows = incr(length, strlen(callingID));
	printf("\033[s");
	printf("\033[%d;1H", row_count);
	printf("\033[1;31m%s: \033[0m", callingID);
	printf("%s", recvBuf);
	printf("\033[u");
	row_count += written_rows;
	
}

void printSendUp(char* sendBuf, int length){
	int written_rows;
	int i;
	written_rows = incr(length, strlen("You"));
	for (i = 0; i < written_rows; i++)
		printf("\033[A\033[2K");
	printf("\033[s");
	printf("\033[%d;1H", row_count);
	printf("\033[1;34m%s: \033[0m", "You");
	printf("%s", sendBuf);
	printf("\033[u");
	row_count += written_rows;

}



//Change socket to NONBLOCKING mode
void noBlockSocket(int ds_sock){	
	int flags = fcntl(ds_sock, F_GETFL, 0);
	fcntl(ds_sock, F_SETFL, flags | O_NONBLOCK);
}


//Change stdin to NONBLOCKING mode
void noBlockInput () {
	int fd = fileno(stdin);
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


//Reset stdin to BLOCKING mode
void resetBlockInput(){
	int fd = fileno(stdin);
	fcntl(fd, F_SETFL, 0);
}


//This combination of escape characters does the following: 
//Erase screen, Moves the cursor back to home position
//Moves the cursor up a line to compensate for puts' line feed
//prints the formatted info about the caller
void stampaHeader(){
	puts("\033[2J\033[H");
	printf("\033[AYou have been called by: \033[1;31m%s\033[0m\n",callingIP);
	printf("Caller's name is: \033[1;31m%s\033[0m\n", callingID);
	printf("Type: '\033[1;32m::h\033[0m' for the list of commands\n\n\n");
	row_count = 5;
	printf("\033[s");
	r = ts.ws_row-3;
	printf("\033[%d;1H", r);
	printf("\033[s");
	
}


//This function manages the commands input by the user
int command(char* buf) {
	int k;
	char letter = buf[2];
	switch(letter){
		case 'H':
		case 'h':
			printf("\033[A\033[2K");
			puts("Welcome to the HELP, here is the list of possbile commands.");
			puts("::a --> Accept incoming call");
			puts("::c --> Call someone");
			puts("::d --> Disconnect from current call");
			puts("::l --> Get the list of available clients form server");
			puts("::q --> Quit the program");
			puts("::h --> Show the HELP");	
			return LISTEN;
		break;
		
		case 'A':
		case 'a':
			return ACCEPT;
		break;
		
		case 'C':
		case 'c':
			if (inchat == INCHAT){
				printf("Please disconnect from current call before attempting a new call");
				return LISTEN;
			}
			else{
				return CONNECT;
			}
		break;
		
		case 'D':
		case 'd':
			if (inchat == NOTINCHAT)
				return LISTEN;
			puts("Disconnecting...");
			sleep(1);
			printf("\033[2J");
			for(k = 0; k < ts.ws_row; k++)
				printf("%s", "\033[A");
			return DISCONNECT;
		break;
		
		case 'L':		
		case 'l':
			if (inchat == INCHAT){
				printf("Please disconnect from current call before attempting a connection to the server");
				return LISTEN;
			}
			return LIST;
		break;
			
		case 'Q':
		case 'q':
			return QUIT;
		break;
	
		default:
			puts("Couldn't recognise the command, please try '::h' for help");
			return LISTEN;
		
	}
}


//This function is used to keep track of changes in the size of the terminal window
void sigwinch_handler (int a) {
	signal(SIGWINCH, sigwinch_handler);
	ioctl(0, TIOCGWINSZ, &ts);
	if(inchat == 1){
//PROBLEM: when resizing downward the screen does not scroll down to compensate
		stampaHeader();
	}

}


//Checks for incoming commands
int checkForCommand(char* sendBuf) {

//-1 is used to compensate for the newline character
	int len = strlen(sendBuf) -1;
	
	if(len == 3 && sendBuf[0] == ':' && sendBuf[1] == ':') {
		com_res = command(sendBuf);
		sendBuf[0] = '\0';
		return 1;
	}
	else if (inchat == 0){
		puts("Type '::h' for HELP");
	}

	return 0;
}



//Chat loop function
void chat(int* sock, char sendBuf[], char recvBuf[]) {
	int rN;
	int sock_a = *sock;

	while(!(com_res < LISTEN)){				
		rN = read(sock_a, recvBuf, DIM);
		if(rN > 0){
			printRecvUp(recvBuf, strlen(recvBuf));
			recvBuf[0] = '\0';
		}
		if((fgets(sendBuf, DIM, stdin)) != NULL) {
			if(acceptableString(sendBuf) == 1){
				if(checkForCommand(sendBuf) != 1){
					printf("\033[2K");
					printSendUp(sendBuf, strlen(sendBuf));
					write(sock_a, sendBuf, DIM);
					sendBuf[0] = '\0';
				}	
			}
			else{
				printSendUp(ESC_ERR, ESC_ERR_LEN);
				sendBuf[0] = '\0';
			}
		}
		if(row_count >= ts.ws_row -3){
				stampaHeader();
		}
	}
}



//This thread manages incoming connections
void func_1 () {

	int servsock;
	int sock_a;
	int control;
	int length;
	int k;
	struct sockaddr_in server;
	struct sockaddr_in client;
	short port = 4000;
	char sendBuf[DIM];
	char recvBuf[DIM];

	
//open the server socket to accept connections
	if ((servsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		perror("Could not Open socket");
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	puts("We created the new socket");
#endif

//fills up the sockaddr_in struct accepting connections from anyone
//to be checked for security reasons 
//(does it have to accept only from a given list?)
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;


//binds the socket with the sockaddr_in struct previously filled
	if ((control = bind(servsock, (struct sockaddr*)&server, sizeof(server)))==-1){
		perror("Could not Bind socket");
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	puts("The binding has been done");
#endif

	if((control = listen(servsock, 5)) == -1){
		perror("Could not Listen on socket");
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	puts("The process is now listening");
#endif

	while(com_res != QUIT && com_res != LIST && com_res != CONNECT){
	
//waits for connections, changing both servsock and stdin to NONBLOCK
		length = sizeof(client);
		puts("Type '::h' for HELP\n\033[1;34mWaiting for incoming calls.....\033[0m");
		noBlockInput();
		noBlockSocket(servsock);
		while ((sock_a = accept(servsock, (struct sockaddr *)&client, &length)) == -1 && com_res != QUIT && com_res != LIST && com_res != CONNECT){
			if((fgets(sendBuf, DIM, stdin)) != NULL)
				checkForCommand(sendBuf);
		}

//Checks if the user selected the "quit" option |||CHECKIT||
		if(com_res == QUIT || com_res == LIST)
			break;

//clears the screen
		printf("\033[2J");
		for(k = 0; k < ts.ws_row; k++)
			printf("%s", "\033[A");
			
//Reset stdin to BLOCKING MODE
		resetBlockInput();
	
//Gets caller's IP and name and sends own name
		callingIP = inet_ntoa(client.sin_addr);
		read(sock_a, recvBuf, DIM);
		strcpy(callingID, recvBuf);
		recvBuf[0] = '\0';
		stampaHeader();
		write(sock_a, userName, DIM);
		

//Asks the user if she/he wants to accept the incoming call
		com_res = LISTEN;	
		inchat = INCHAT;
		printf("Do you want to accept the incoming call?\n");
		while (com_res == LISTEN) {
			if((fgets(sendBuf, DIM, stdin)) != NULL)
				checkForCommand(sendBuf);
			sendBuf[0] = '\0';
		}
		if(com_res == ACCEPT){
			
//sends accept confirmation
			sendBuf[0] = 1;
			write(sock_a, userName, DIM);
			sendBuf[0] = '\0';
			
//change the communicating socket and STDIN to NONBLOCKING mode
			noBlockSocket(sock_a);
			noBlockInput();
	

			stampaHeader();
	
			
//Chat loop
			chat(&sock_a, sendBuf, recvBuf);
		}
		else if (com_res != QUIT){
			sendBuf[0] = 0;
			write(sock_a, userName, DIM);
			sendBuf[0] = '\0';
			close(sock_a);
			puts("Call refused");
			com_res = LISTEN;
			inchat = NOTINCHAT;
		}
	}
	close(servsock);
}




