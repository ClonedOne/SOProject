#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIM 1024
#define MAX_CODA 3

void main(){

	int ds_sock;
	int length;
	int res;
	struct sockaddr_in client;
	struct hostent *hp;
	char buff[MAX_DIM];

//crea la socket per inviare dati
	ds_sock = socket (AF_INET, SOCK_STREAM, 0);
	
	client.sin_family = AF_INET;
	client.sin_port = htons(4000);

	if ( (hp = gethostbyname("localhost")) == NULL){
		puts("FAAK LOCALHOST");
	}
	bcopy(hp->h_addr, &client.sin_addr, hp->h_length);

//	memcpy(&client.sin_addr, hp->h_addr_list[0], hp->h_length);
	
	res = connect(ds_sock,(struct sockaddr *)&client,sizeof(client));
	if (res == -1){
		puts("ERRORE CONNECT!!");
	}
	
	printf("stringhe\n");
	read(ds_sock, buff, MAX_DIM);
	printf("%s\n", buff);
	scanf("%s", buff);
	write(ds_sock, buff, MAX_DIM);
}

