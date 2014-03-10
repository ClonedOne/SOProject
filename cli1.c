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

#define DIM 1024
struct winsize ts;
char* callingIP;
char callingID[DIM];
int row_count;
int inchat;



//This function is used to keep track of changes in the size of the terminal window
void sigwinch_handler (int a) {
	signal(SIGWINCH, sigwinch_handler);
	ioctl(0, TIOCGWINSZ, &ts);
	if(inchat == 1){
	//PROBLEM: when the resizing is downward, while it still does the job, the screen does not scroll down to compensate
	//This combination of escape characters does the following: 
	//Erase screen, Moves the cursor back to home position, Moves the cursor up a line to compensate for puts' line feed
		puts("\033[2J\033[H");
		printf("\033[AYou have been called by: %s\n",callingIP);
		printf("Caller name is: %s\n", callingID);
		printf("Type: '-c h' for the list of commands\n\n");
		row_count = 0;
	}
}



//This thread manages incoming connections
void* func_t_1 () {

	int servsock;
	int sock_a;
	int control;
	int length;
	struct sockaddr_in server;
	struct sockaddr_in client;
	short port = 4000;
	char sendBuf[DIM];
	char recvBuf[DIM];
	char* comm;
	
	
	//this struct is used to store the dimensions (rows/col) of the terminal window
	ioctl(0, TIOCGWINSZ, &ts);
	signal(SIGWINCH, sigwinch_handler);
	row_count = 0;
	
	
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


	//waits for connections
	length = sizeof(client);
	puts("We are waiting to accept a connection from the other users");
	while ((sock_a = accept(servsock, (struct sockaddr *)&client, &length)) == -1);
	puts("Connection Accepted");


	callingIP = inet_ntoa(client.sin_addr);
	printf("You have been called by: %s\n",callingIP);

	
	//I'll probably will let the server/other client do this part of exchanging names
	comm = "Hello, please state your name";
	strcpy(sendBuf, comm);
	//printf("DEBUG: %s\n", comm);
	write(sock_a, sendBuf, DIM);
	read(sock_a, recvBuf, DIM);
	strcpy(callingID, recvBuf);
	printf("caller's name is: %s\n", callingID);
	printf("Type: '-c h' for the list of commands\n\n");
	
	while (1){
		inchat = 1;
		
		//I need to find a way to make "read" non blocking and possibly interrupt driven
		printf( "%-*s\n", ts.ws_col, "ABC" );
		
		
		row_count++;	
		if (row_count == ts.ws_row -4){
			puts("\033[2J\033[H");
			printf("\033[AYou have been called by: %s\n",callingIP);
			printf("Caller name is: %s\n", callingID);
			printf("Type: '-c h' for the list of commands\n\n");
			 row_count = 0;
		}
	
	
	}

}

void* func_t_2 (){}

int main(int argc, char*argv[]){

	pthread_t t1;
	int retT1;

	puts("Welcome! We are now starting the service!");
	retT1 = pthread_create(&t1, NULL, &func_t_1, NULL);
	if (retT1 != 0)
		puts("Couldn't create the thread!");
	while(1);//STOOOOOOOOPIDO!
}
