/* This is a client program. U DONT SAY?!
It consists of 2 threads
	-The first waits for connections from other programs just like this and eventually start a chat with it
	-The second tries to talk to a server and get the address of another program just like this form a list provided by the server itself
The goal of all of it is just to create a sinchronous communication channel between two users over the net.*/

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

#define DIM 1024
#define ACCEPT 1
#define DISCONNECT -2
#define QUIT -1

struct winsize ts;
char* callingIP;
char callingID[DIM];
int row_count;
int inchat;
int com_res;



//This combination of escape characters does the following: 
//Erase screen, Moves the cursor back to home position
//Moves the cursor up a line to compensate for puts' line feed
//prints the formatted info about the caller
void stampaHeader(){
	puts("\033[2J\033[H");
	printf("\033[AYou have been called by: \033[1;31m%s\033[0m\n",callingIP);
	printf("Caller's name is: \033[1;31m%s\033[0m\n", callingID);
	printf("Type: '\033[1;32m::h\033[0m' for the list of commands\n\n");
	row_count = 0;
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
			puts("::c --> Call someone"); //TO BE DONE
			puts("::d --> Disconnect from current call");
			puts("::q --> Quit the program");
			puts("::h --> Show the HELP");	
		break;
		
		
		case 'A':
		case 'a':
			return ACCEPT;
		break;
		
		
		case 'D':
		case 'd':
			puts("Disconnecting...");
			sleep(1);
			printf("\033[2J");
			for(k = 0; k < ts.ws_row; k++)
				printf("%s", "\033[A");
			return DISCONNECT;
		break;
	
		case 'Q':
		case 'q':
			return QUIT;
		break;
	
		default:
			puts("Couldn't recognise the command, please try '::h' for help");
		
	}
	return 0;
}


//This function is used to keep track of changes in the size of the terminal window
void sigwinch_handler (int a) {
	signal(SIGWINCH, sigwinch_handler);
	ioctl(0, TIOCGWINSZ, &ts);
	if(inchat == 1){
	//PROBLEM: when the resizing is downward the screen does not scroll down to compensate
		stampaHeader();
	}
}



//This thread manages incoming connections
void* func_t_1 () {

	int servsock;
	int sock_a;
	int control;
	int length;
	int rN;
	int wN;
	int k;
	struct sockaddr_in server;
	struct sockaddr_in client;
	short port = 4000;
	char sendBuf[DIM];
	char recvBuf[DIM];
	
	
	puts("We are starting the receiving thread");
	
	//open the server socket to accept connections
	if ((servsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		puts("Could not Open socket");
		exit(EXIT_FAILURE);
	}
	puts("We created the new socket");


	//fills up the sockaddr_in struct accepting connections from anyone
	//to be checked for security reasons (does it have to accept only from a given list?)
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;


	//binds the socket with the sockaddr_in struct previously filled
	if ((control = bind(servsock, (struct sockaddr*)&server, sizeof(server)))==-1){
		puts("Could not Bind socket");
		exit(EXIT_FAILURE);
	}
	puts("The binding has been done");

	if((control = listen(servsock, 5)) == -1){
		puts("Could not Listen on socket");
		exit(EXIT_FAILURE);
	}
	puts("The process is now listening");

	while(com_res != QUIT){
	
		//waits for connections
		length = sizeof(client);
		puts("\033[1;34mWaiting for incoming calls.....\033[0m");
		while ((sock_a = accept(servsock, (struct sockaddr *)&client, &length)) == -1 );
		
		//clears the screen
		printf("\033[2J");
		for(k = 0; k < ts.ws_row; k++)
			printf("%s", "\033[A");
	
		//Gets caller's IP and name and ask the user to accept the call
		callingIP = inet_ntoa(client.sin_addr);
		read(sock_a, recvBuf, DIM);
		strcpy(callingID, recvBuf);
		recvBuf[0] = '\0';
		stampaHeader();
		printf("Do you want to accept the incoming call?\n");
		fgets(sendBuf, DIM, stdin);
		com_res = command(sendBuf);
		sendBuf[0] = '\0';
	
		if(com_res == ACCEPT){
	
			//change the communicating socket and STDIN to NONBLOCKING mode
			int flags = fcntl(sock_a, F_GETFL, 0);
			fcntl(sock_a, F_SETFL, flags | O_NONBLOCK);
			int fd = fileno(stdin);
			flags = fcntl(fd, F_GETFL, 0);
			fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	
			inchat = 1;
			raise(SIGWINCH);
	
			while(!(com_res <= 0)){
				rN = read(sock_a, recvBuf, DIM);
				if(rN > 0){
					printf("%s", "\033[2K");
					for(k = 0; k < ts.ws_col; k++)
						printf("%s", "\033[D");
					printf("\033[1;31m%s: \033[0m", callingID);
					printf("%s", recvBuf);
					row_count++;
					recvBuf[0] = '\0';
				}
				if((fgets(sendBuf, DIM, stdin)) != NULL) {
					if(sendBuf[0] == ':' && sendBuf[1] == ':') {
						com_res = command(sendBuf);
						sendBuf[0] = '\0';
					}else{
						printf("\033[A\033[2K");
						printf("\033[1;34m%s: \033[0m", "You");
						printf("%s", sendBuf);
						write(sock_a, sendBuf, DIM);
						sendBuf[0] = '\0';
						row_count++;
					}
				}
		
				if(row_count == ts.ws_row -4){
					stampaHeader();
				}
	
			}
		}
		else{
			close(sock_a);
			puts("Call refused");
		}
	}
}

void* func_t_2 (){}

int main(int argc, char*argv[]){

	pthread_t t1;
	int retT1;
	int k;

	//this struct is used to store the dimensions (rows/col) of the terminal window
	ioctl(0, TIOCGWINSZ, &ts);
	signal(SIGWINCH, sigwinch_handler);
	row_count = 0;

	printf("\033[2J");
	for(k = 0; k < ts.ws_row; k++)
	printf("%s", "\033[A");
	
	
	puts("Welcome! We are now starting the service!");
	retT1 = pthread_create(&t1, NULL, &func_t_1, NULL);
	if (retT1 != 0)
		puts("Couldn't create the thread!");
		
	//This loop ends when the user input the ::q command
	while(com_res != QUIT);
}
