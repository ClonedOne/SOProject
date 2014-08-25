#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>

void main() {

	char sendBuf[1024];
	char callingIP[1024];
	char* ip;
	int sock;
	struct sockaddr_in client;
	
	ip = "127.0.0.1";
	
	printf("Please select the user\n");
	fgets(callingIP, 1024, stdin);
	callingIP[strlen(callingIP)-1] = '\0';
	printf("chosen ip is %s!\n",callingIP);

	
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		perror("Could not Open socket");

	}

	client.sin_family = AF_INET;
	client.sin_port = htons(4100);
	if (inet_aton(callingIP, &client.sin_addr) == 0) {
		perror("Address to network conversion error");
	}
	

	if (connect(sock,(struct sockaddr *)&client,sizeof(client)) == -1){
		perror("Couldn't connect");
	}
	



	write(sock, "ciao", 1024);







}
