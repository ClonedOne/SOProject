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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include "cli1.h"


#define DEBUG


struct winsize ts;
char* callingIP; 			//IP address of the caller
char callingID[IDLEN]; 		//User Name of the caller
char userName[DIM]; 		//My User Name
char userPwd[DIM]; 			//My Password
char serverCom[SERVCOMLEN];	//Buffer used to communicate with server
int row_count; 				//number of rows of the terminal window
int inchat; 				//flag that shows that client is currently chatting
int called; 				//flag that shows that client is currently being called
int calling;				//flag that shows that client is currently calling someone
int com_res; 				//store the result of the command function
int r;
sem_t semaphore;			//semaphore used to communicate between threads
pthread_t t1, t2, t3; 		//thread handles



//This thread connects to the server and dwonload the clients list
void* func_t_2 (){

	int sock;
	int dest_file;
	char* fd_name;
	char buff[DIM];
	struct sockaddr_in client;
	
	fd_name = "listinaD.txt";
	
	

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
	if (inet_aton("127.0.0.1", &client.sin_addr) == 0) {
		perror("Address to network conversion error");
	}
	
//Creates the destination file
	if ((dest_file = open(fd_name, O_WRONLY|O_CREAT|O_TRUNC,0660)) == -1){
		perror("error creating destination file");
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
	sem_wait(&sem_name); 
	char letter = serverCom[0];
	switch(letter){
		case '0':
			write(sock, serverCom, SERVCOMLEN);
			
		break;
		
		
		case '1':
		
		break;
		
		case '2':
		
		break;
		
		case '3':
		
		break;
		
		case '4':		
		
		break;
			
		case '5':
		
		break;
	
		default:
			return;
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	int size;
	do {
		if((size = read(sock, buff, DIM)) == -1){
			perror("read error");
		}
		if (size > 0)
			printf("%s\n",buff);
		if((write(dest_file, buff, size)) == -1){
			perror("write error");
		}
	}while(size > 0);
	
	close(dest_file);
}
}



//THE MAIN FUNCTION

int main(int argc, char*argv[]){

	int retT2;		//server talking thread return value
	
	inchat = NOTINCHAT;			
	callingIP = malloc(IPLEN);

//this struct is used to store the dimensions (rows/col) of the terminal window
	ioctl(0, TIOCGWINSZ, &ts);
	signal(SIGWINCH, sigwinch_handler);
	row_count = 0;

//Spawn server talking thread adn create communication semaphore
	if (sem_init(&sempahore, 0, SEMZERO) == -1)
		perror("Couldn't create the semaphore");
	retT2 = pthread_create(&t2, NULL, &func_t_2, NULL);
	if (retT2 != 0)
		perror("Couldn't create the thread!");

//clear screen and ask for command
	clearS();
	puts("Welcome! We are now starting the service!");
	printf("Please select which action to perform:\n Write '0' to Sign Up as a new user\n Write '1' to Log In\n");
	fgets(serverCom, SERVCOMLEN, stdin);
	while (serverCom[0] != '1' && serverCom[0] != '0'){
		puts("Invalid command please repeat your selection");
		serverCom[0] = '\0';
		fgets(serverCom, SERVCOMLEN, stdin);
	}
	if (serverCom[0] == '0')
		signUpFunc();
	logInFunc();


	do{
		userName[0] = '\0';
		puts("Please insert your User Name");
		fgets(userName, DIM, stdin);
	} while (acceptableString(userName) == 0);
	userName[strlen(userName) -1] = '\0';
	
	do{
		userPwd[0] = '\0';
		puts("Please insert your Password");
		fgets(userPwd, DIM, stdin);
	} while (acceptableString(userPwd) == 0);
	userPwd[strlen(userPwd) -1] = '\0';
	
	
		
//This loop ends when the user input the ::q command
	while(com_res != QUIT);
	printf("\033[1;31mQuitting....\033[0m\n");
	sleep(2);
	clearS();
	exit(1);
}


//Log in function 
void logInFunc() {
}

//Sign up function 
void signUpFunc() {
}


//this thread is used to connect to other clients
void* func_t_3() {
	
	
	calling = 1;
	int sock;
	int length;
	int res;
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

	inchat = 1;
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




//Spawns threead 2 for server/client communications
void spawnT2 () {
	int retT2;
	retT2 = pthread_create(&t2, NULL, &func_t_2, NULL);
	if (retT2 != 0)
		perror("Couldn't create the thread!");

}


//Spawns thread 3 for client/client communications
void spawnT3 () {
	int retT3;
	retT3 = pthread_create(&t3, NULL, &func_t_3, NULL);
	if (retT3 != 0)
		perror("Couldn't create the thread!");

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
	int k;
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
	int k;	
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
			puts("::c --> Call someone"); //TO BE DONE
			puts("::d --> Disconnect from current call");
			puts("::l --> Get the list of available clients form server");
			puts("::q --> Quit the program");
			puts("::h --> Show the HELP");	
			return 0;
		break;
		
		case 'A':
		case 'a':
			return ACCEPT;
		break;
		
		case 'C':
		case 'c':
			if (called == 1 || calling == 1){
				printf("Please disconnect from current call before attempting a new call");
				return 0;
			}
			else{
				spawnT3();
				return 0;
			}
		break;
		
		case 'D':
		case 'd':
			if (called == 0)
				return 0;
			puts("Disconnecting...");
			sleep(1);
			printf("\033[2J");
			for(k = 0; k < ts.ws_row; k++)
				printf("%s", "\033[A");
			return DISCONNECT;
		break;
		
		case 'L':		
		case 'l':
			spawnT2();
			return 0;
		break;
			
		case 'Q':
		case 'q':
			return QUIT;
		break;
	
		default:
			puts("Couldn't recognise the command, please try '::h' for help");
			return 0;
		
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

	while(!(com_res < 0)){				
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
void* func_t_1 () {

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

	
#ifdef DEBUG	
	puts("We are starting the receiving thread");
#endif	
	
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

	while(com_res != QUIT){
	
//waits for connections, changing both servsock and stdin to NONBLOCK
		length = sizeof(client);
		puts("Type '::h' for HELP\n\033[1;34mWaiting for incoming calls.....\033[0m");
		noBlockInput();
		noBlockSocket(servsock);
		while ((sock_a = accept(servsock, (struct sockaddr *)&client, &length)) == -1 && com_res != QUIT){
			if((fgets(sendBuf, DIM, stdin)) != NULL)
				checkForCommand(sendBuf);
		}

//Checks if the user selected the "quit" option
		if(com_res == QUIT)
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
		com_res = 0;	
		called = 1;
		printf("Do you want to accept the incoming call?\n");
		while (com_res == 0) {
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
	
			inchat = 1;
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
			com_res = 0;
			inchat = 0;
			called = 0;
		}
	}
	close(servsock);
}




