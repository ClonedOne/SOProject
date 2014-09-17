#ifndef CLI1
#define CLI1


#define DIM (1024)
#define CONNECT (3)
#define LIST (2)
#define ACCEPT (1)
#define LISTEN (0)
#define DISCONNECT (-2)
#define QUIT (-1)
#define ESC_ERR "\033[1;34mPlease insert only printable characters\033[0m"
#define ESC_ERR_LEN (40)
#define NOTINCHAT (0)
#define INCHAT (1)
#define IPLEN (32)
#define IDLEN (128)
#define USERNAME (32)
#define PASSWORD (32)
#define SEMZERO (0)
#define SEMONE (1)
#define SERV_COM (8)


//Clear Screen
void clearS ();


//Client thread
//This thread connects to the server and dwonload the clients list
void* func_t_2 ();


//Finds out the increment of lines caused by the new message string
//"1" is used to compensate for strings of the exact length of the terminal
int incr (int slen, int nlen);


//Saves current cursor postiion
//force the cursor on the current wiriting line 
//prints the received string and increments the current writing line
//restore previous cursor position
void printRecvUp(char* recvBuf, int length);
void printSendUp(char* sendBuf, int length);


//Change socket to NONBLOCKING mode
void noBlockSocket(int ds_sock);


//Change stdin to NONBLOCKING mode
void noBlockInput ();


//Reset stdin to BLOCKING mode
void resetBlockInput();


//This combination of escape characters does the following: 
//Erase screen, Moves the cursor back to home position
//Moves the cursor up a line to compensate for puts' line feed
//prints the formatted info about the caller
void stampaHeader();


//This function manages the commands input by the user
int command(char* buf);


//This function is used to keep track of changes in the size of the terminal window
void sigwinch_handler (int a);


//Checks for incoming commands
int checkForCommand(char* sendBuf);


//Chat loop function
void chat(int* sock, char sendBuf[], char recvBuf[]);



//This function manages incoming connections
void func_1 ();


//This function manages outgoing calls
void func_3 ();


//check if the string contains unacceptable characters
int acceptableString(char *s);


//Log in function
void logInFunc();

//Sign up function
void signUpFunc();



#endif
