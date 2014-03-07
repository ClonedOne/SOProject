/* This is a client program. U DONT SAY?!
It consists of 2 threads
	-The first waits for connections from other programs just like this and 	eventually start a chat with it
	-The second tries to talk to a server and get the address of another 		program just like this form a list provided by the server itself
The goal of all of it is just to create a sinchronous communication channel between two users over the net.
*/
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



void* func_t_1 () {
	int servsock;
	int sock_a;
	int control;
	int length;
	struct sockaddr_in server;
	struct sockaddr_in client;
	//struct in_addr tempAddr;
	short port = 4000;
	char contBuf[DIM];
	char* comm;
	char* callingIP;
	
	//this struct is used to store the dimensions (rows/col) of the terminal window
	struct winsize ts;
	ioctl(0, TIOCGWINSZ, &ts);
	
	puts("We are starting the receivign thread");
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
	printf("You have been called by: %s\n",callingIP); //inet_ntoa(client.sin_addr));

	comm = "Hello, please state your name";
	strcpy(contBuf, comm);
	//printf("DEBUG: %s\n", comm);
	write(sock_a, contBuf, DIM);
	read(sock_a, contBuf, DIM);
	printf("caller's name is: %s\n", contBuf);
	
	int row_count = 0;
	while (1){
	//DO NOT RESIZE WINDOW OR THERE WILL BE PROBLEMS
	//TO BE FIXED SOON
	sleep(1);
	printf( "%-*s\n", ts.ws_col, "ABC" );
	row_count++;
	
	if (row_count == ts.ws_row -2){
		system("/usr/bin/clear");
		printf("You have been called by: %s\n",callingIP);
		printf("caller's name is: %s\n", contBuf);
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
