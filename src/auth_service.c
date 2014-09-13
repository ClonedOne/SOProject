#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/sem.h>
#include "commandManager.h"
#include "login_parser.h"
#include "concurrent_hashmap.h"

char* ack = ACK_FROM_SERVER;
char* refuse = REFUSE_FROM_SERVER;
char* error = ERROR_FROM_SERVER;

int registrationRequest(int* cli_sock)
{
	
	

	char* user = malloc(MAX_USERNAME_LENGTH);
	char* password = malloc(MAX_PASSWORD_LENGTH);
	int ipAddr;
	struct sockaddr_in* sockaddr;
	char* ip=malloc(INET_ADDRSTRLEN);
	struct sembuf oper[2];

	
	int check_registered;
	int registered;
	


	//SEND ACK TO CLIENT FOR REGISTRATION DATA SENDING
	
	
	
	if ((write(cli_sock, ack, ACK_FROM_SERVER_LENGTH)) == -1)
	{
		perror("Write ACK for registration error");
		return -1;
	}

	//WAIT FOR USER REGISTRATION DATA

	do {
		if ((read(cli_sock, user, MAX_USERNAME_LENGTH)) == -1){
			perror("Read user error");
			return -1;
		}

		//IF REGISTRATION REFUSED NOTIFY CLIENT
		
		if(check_registered=checkUserRegistered(user) == 1)
		{
			if ((write(cli_sock, refuse, REFUSE_FROM_SERVER_LENGTH)) == -1)
			perror("Write server refuse error");
			return -1;		
					
		}
	}
	while(check_registered==0);
	//GET PASSWORD FROM CLIENT	

	

	if ((read(cli_sock, password, MAX_PASSWORD_LENGTH)) == -1){
		perror("Read password error");
		return -1;
		
	}

	//REGISTER USER	

	if(clientRegistration(user,password)==-1)
	{
		
		if ((write(cli_sock, error, ERROR_FROM_SERVER_LENGTH)) == -1)
			perror("Registration server error error");		
		return -1;		
	}

	if ((write(cli_sock, ack, ACK_FROM_SERVER_LENGTH)) == -1)
	{
		perror("Write ACK for succesful registration error");
		return -1;
	}	

	//RETRIEVE USER IP 

	sockaddr = (struct sockaddr_in*)&cli_sock;
	ipAddr= sockaddr->sin_addr.s_addr;
	
	inet_ntop( AF_INET, &ipAddr, ip, INET_ADDRSTRLEN );
	
	oper[0].sem_num = 0;    
    	oper[0].sem_op  = 0;    
    	oper[0].sem_flg = 0;

    	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di sincronizzazione sul semaforo");
	
	//PUT IN HASHMAP AS AVAILABLE

	putIntoHashmap(hashmap, user, ip, "available");

	//SAVE STATUS INTO LOG FILE	

	saveInLog(user,ip,"available");

	oper[0].sem_num = 0;    
	oper[0].sem_op =  1;    
	oper[0].sem_flg = 0;

	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di incremento del semaforo dopo l'inserimento nell'hashmap dello stato utente");

	return 1;
   

	
	

	
}

int loginRequest(int* cli_sock)
{
	int logged=0;
	char** info=malloc(sizeof(char*)*1);
	char* user = malloc(MAX_USERNAME_LENGTH);
	char* password = malloc(MAX_PASSWORD_LENGTH);
	int ipAddr;
	struct sockaddr_in* sockaddr;
	char* ip=malloc(INET_ADDRSTRLEN);
	struct sembuf oper[1];

	
	int check_registered;
	int registered;

	//SEND ACK TO CLIENT FOR REGISTRATION DATA SENDING
	
	
	
	if ((write(cli_sock, ack, ACK_FROM_SERVER_LENGTH)) == -1)
	{
		perror("Write ACK for login error");
		return -1;
	}

	
	//WAIT FOR USER LOGIN DATA

	do {
		if ((read(cli_sock, user, MAX_USERNAME_LENGTH)) == -1){
			perror("Read user error");
			return -1;
		}
		

		if ((read(cli_sock, password, MAX_PASSWORD_LENGTH)) == -1){
		perror("Read password error");
		return -1;
		}

		//CHECK LOGIN

		getInfoByUser(user,info);
		if(strcmp(password,info[0])==0)
			logged=1;
		else
		{
			//REFUSES IF NOT LOGGED
			if ((write(cli_sock, refuse, REFUSE_FROM_SERVER_LENGTH)) == -1)
			perror("Write server refuse error");
			return -1;	
			
		}
	}		
	while(logged=1);

	//RETRIEVE USER IP 

	sockaddr = (struct sockaddr_in*)&cli_sock;
	ipAddr= sockaddr->sin_addr.s_addr;
	
	inet_ntop( AF_INET, &ipAddr, ip, INET_ADDRSTRLEN );
	
	oper[0].sem_num = 0;    
    	oper[0].sem_op  = 0;    
    	oper[0].sem_flg = 0;

    	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di sincronizzazione sul semaforo");
	
	//PUT IN HASHMAP AS AVAILABLE

	putIntoHashmap(hashmap, user, ip, "available");

	//SAVE STATUS INTO LOG FILE	

	saveInLog(user,ip,"available");

	oper[0].sem_num = 0;    
	oper[0].sem_op =  1;    
	oper[0].sem_flg = 0;

	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di incremento del semaforo dopo l'inserimento nell'hashmap dello stato utente");

	return 1;

	
}

int logoutRequest(char* user, int* cli_sock)
{
	struct sembuf oper[1];
	oper[0].sem_num = 0;    
    	oper[0].sem_op  = 0;    
    	oper[0].sem_flg = 0;

    	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di sincronizzazione sul semaforo");
	
	//DELETES FROM HASHMAP

	deleteFromHashmap(hashmap, user);

	//SAVE STATUS INTO LOG FILE	

	deleteFromLog(user);

	oper[0].sem_num = 0;    
	oper[0].sem_op =  1;    
	oper[0].sem_flg = 0;

	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di incremento del semaforo dopo l'inserimento nell'hashmap dello stato utente");

	return 1;
}

int availableRequest(char* user, int* cli_sock)
{
	struct sembuf oper[1];
	char* ip=malloc(INET_ADDRSTRLEN);

	struct sockaddr_in* sockaddr = (struct sockaddr_in*)&cli_sock;
	int ipAddr= sockaddr->sin_addr.s_addr;
	
	inet_ntop( AF_INET, &ipAddr, ip, INET_ADDRSTRLEN );

	oper[0].sem_num = 0;    
    	oper[0].sem_op  = 0;    
    	oper[0].sem_flg = 0;

    	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di sincronizzazione sul semaforo");
	
	//DELETES FROM HASHMAP

	deleteFromHashmap(hashmap, user);

	//PUT IN HASHMAP USER WITH LOGIN STATUS

	putIntoHashmap(hashmap, user, ip, "available");


	//SAVE STATUS INTO LOG FILE	

	//DA OTTIMIZZARE 
	
	deleteFromLog(user);

	saveInLog(user,ip,"available");

	oper[0].sem_num = 0;    
	oper[0].sem_op =  1;    
	oper[0].sem_flg = 0;

	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di incremento del semaforo dopo l'inserimento nell'hashmap dello stato utente");

	return 1;
}

int busyRequest(char* user, int* cli_sock)
{
	struct sembuf oper[1];
	char* ip=malloc(INET_ADDRSTRLEN);
	struct sockaddr_in* sockaddr = (struct sockaddr_in*)&cli_sock;
	int ipAddr= sockaddr->sin_addr.s_addr;
	
	inet_ntop( AF_INET, &ipAddr, ip, INET_ADDRSTRLEN );

	oper[0].sem_num = 0;    
    	oper[0].sem_op  = 0;    
    	oper[0].sem_flg = 0;

    	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di sincronizzazione sul semaforo");
	
	//DELETES FROM HASHMAP

	deleteFromHashmap(hashmap, user);

	//PUT IN HASHMAP USER WITH LOGIN STATUS

	putIntoHashmap(hashmap, user, ip, "available");


	//SAVE STATUS INTO LOG FILE	

	//DA OTTIMIZZARE 
	
	deleteFromLog(user);

	saveInLog(user,ip,"busy");

	oper[0].sem_num = 0;    
	oper[0].sem_op =  1;    
	oper[0].sem_flg = 0;

	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di incremento del semaforo dopo l'inserimento nell'hashmap dello stato utente");

	return 1;
}

int retrieveListRequest(int*cli_sock)
{
	int logged=0;
	char** info=malloc(sizeof(char*)*1);
	char* user = malloc(MAX_USERNAME_LENGTH);
	char* password = malloc(MAX_PASSWORD_LENGTH);
	char* newLine="\n";
	int i;
	int ipAddr;
	struct sockaddr_in* sockaddr;
	char* ip=malloc(INET_ADDRSTRLEN);
	struct sembuf oper[1];
	
	oper[0].sem_num = 0;    
    	oper[0].sem_op  = 0;    
    	oper[0].sem_flg = 0;

	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di sincronizzazione del semaforo dopo la richiesta della lista");

	value** retrieved = hashmap_retrieveAvailableUsers(hashmap);

	for(i=0;i<hashmap_size;i++)
	{
		if ((write(cli_sock, (*retrieved)->user, sizeof((*retrieved)->ip) == -1)))
		{
			perror("Write user for available list error ");
			return -1;
		}

		if ((write(cli_sock, (*retrieved)->ip, sizeof((*retrieved)->ip) == -1)))
		{
			perror("Write ip for available list error ");
			return -1;
		}

		if((write(cli_sock, newLine, sizeof(newLine) == -1)))
		{
			perror("Errore in scrittura newLine nel file l'invio della lista utenti disponibili al client");
			return -1;	
		}
		
	}

	oper[0].sem_num = 0;    
	oper[0].sem_op =  1;    
	oper[0].sem_flg = 0;
	
	
	
	if(semop(hashmap_sem, oper, 1)==-1)
		perror("Errore di incremento del semaforo dopo l'invio della lista utenti disponibili al client");

	return 1;

}
