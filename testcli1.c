#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>


#define MAX_DIM 1024
#define MAX_CODA 3

void main(){

	int ds_sock;
	int length;
	int res;
	struct sockaddr_in client;
	struct hostent *hp;
	char buff[MAX_DIM];
	char* name = "ClonedTest1";
//crea la socket per inviare dati
	ds_sock = socket (AF_INET, SOCK_STREAM, 0);
	
	client.sin_family = AF_INET;
	client.sin_port = htons(4000);

	if ( (hp = gethostbyname("localhost")) == NULL){
		puts("FAAK LOCALHOST");
	}
	bcopy(hp->h_addr, &client.sin_addr, hp->h_length);
	
	res = connect(ds_sock,(struct sockaddr *)&client,sizeof(client));
	if (res == -1){
		puts("ERRORE CONNECT!!");
	}
	
	write(ds_sock, name, strlen(name));
	
	
	//change the communicating socket to NONBLOCKING mode
	int flags = fcntl(ds_sock, F_GETFL, 0);
	fcntl(ds_sock, F_SETFL, flags | O_NONBLOCK);
	int rN;
	int fd = fileno(stdin);
	flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	
	while(1){
		rN = read(ds_sock, buff, MAX_DIM);
		if (rN > 0){
			printf("%s\n", buff);
			buff[0] = '\0';
		}
	
		if((fgets(buff, MAX_DIM, stdin)) != NULL) {
			write(ds_sock, buff, MAX_DIM);
			buff[0] = '\0';
		}
	}
	close(ds_sock);
}

