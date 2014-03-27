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


#define CODA 5
#define DIM 1024

void main() {


	int serv_sock;
	int cli_sock;
	int rval;
	int length;
	struct sockaddr_in server;
	struct sockaddr client;
	
	
	puts("Welcome to Such Chatz server!");
	
//open the server socket to accept connections
	if ((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		perror("Could not Open socket");
		exit(EXIT_FAILURE);
	}
	puts("We created the new socket");

//fills in the sockaddr struct 
	server.sin_family = AF_INET;
	server.sin_port = htons(4000);
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





}
