/*file: server_functions.c
**authors: Brian Nguyen, Chance Ball
**date: 4/20/17
**description: this is the source code for server_funtions the first delieverable. This file will contain any functions that are relevant and useful to the server.c code and will help with elegance.
**NOTE: The code that we wrote is a modification on the code that can be found at the following site:  http://www.linuxhowtos.org/C_C++/socket.htm
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

int logportno = 0;

 void error(const char *msg) //Prints an error on system call from stderr
{
	perror(msg);
	exit(1);
}

void setlogportno(int lp){
	logportno = lp;
}
int getlogportno(){
	return logportno;
}

void dostuffTCP(int sockTCP, int sockLog, struct sockaddr_in cli_addr, struct sockaddr_in log_addr){
	int n, k, length, socklog, portno;
	struct sockaddr_in logserver;
	char* cli_ip = inet_ntoa(cli_addr.sin_addr);	//contains ip address of the client
	
	char buffer[256];
	char logMsg[1024];	//will contain the message sent to log_s
	
	//change to 9999 upon submission
	portno = getlogportno();
      
	bzero(buffer,256);
	bzero(logMsg, 1024);
      
	if ((n = read(sockTCP,buffer,255)) < 0)
		error("ERROR reading from socket");
	
	int logMsg_size = strlen(logMsg);	
	
	//creating a socket that will be used to connect to log_s
	socklog = socket(AF_INET, SOCK_DGRAM, 0);
	if (socklog < 0) error("ERROR creating socket");
	 
	logserver.sin_family = AF_INET;	 
	logserver.sin_port = htons(portno);
	 
	length = sizeof(struct sockaddr_in);
	
	strcat(logMsg, "\"");
	strcat(logMsg, buffer);
	
	logMsg[logMsg_size - 1] = '\0';	//terminate with null character
	
	strcat(logMsg, "\" was received from ");
	strcat(logMsg, cli_ip);
	strcat(logMsg, "\n");
	 
	k = sendto(socklog, logMsg, strlen(logMsg), 0, (const struct sockaddr *)&logserver, length);
	 
	if (k < 0) error("ERROR sending to log_s");
	
   
	printf("Your message (TCP): %s", buffer);
   
	if ((n = write(sockTCP, buffer, 255)) < 0)
		error("ERROR writing (TCP)");
}

void dostuffUDP(int sockUDP, int sockLog, socklen_t clilen, struct sockaddr_in cli_addr, struct sockaddr_in log_addr){
	int n, k, length, socklog, portno;
	struct sockaddr_in logserver;
	char* cli_ip = inet_ntoa(cli_addr.sin_addr);	//contains ip address of the client
	
    char buffer[256];
	char logMsg[1024];	//will contain the message sent to log_s
	
	//change to 9999 upon submission
	portno = getlogportno();
      
	bzero(buffer,256);
	bzero(logMsg, 1024);
	
	if ((n = recvfrom(sockUDP, buffer, 255, 0, (struct sockaddr *)&cli_addr, &clilen)) < 0)
		error("ERROR reading (UDP)");
	
	int logMsg_size = strlen(logMsg);	
	
	//creating a socket that will be used to connect to log_s
	socklog = socket(AF_INET, SOCK_DGRAM, 0);
	if (socklog < 0) error("ERROR creating socket");
	 
	logserver.sin_family = AF_INET;	 
	logserver.sin_port = htons(portno);
	 
	length = sizeof(struct sockaddr_in);
	
	strcat(logMsg, "\"");
	strcat(logMsg, buffer);
	
	logMsg[logMsg_size - 1] = '\0';	//terminate with null character
	
	strcat(logMsg, "\" was received from ");
	strcat(logMsg, cli_ip);
	strcat(logMsg, "\n");
	
	//creating a socket that will be used to connect to log_s
	socklog = socket(AF_INET, SOCK_DGRAM, 0);
	if (socklog < 0) error("ERROR creating socket");
	 
	logserver.sin_family = AF_INET;	 
	logserver.sin_port = htons(portno);
	 
	length = sizeof(struct sockaddr_in);
	 
	k = sendto(socklog, buffer, strlen(buffer), 0, (const struct sockaddr *)&logserver, length);
	 
	if (k < 0) error("ERROR sending to log_s");
	
	
	printf("Your message (UDP): %s", buffer);
	
	if ((n = sendto(sockUDP, buffer, 255, 0 , (struct sockaddr *)&cli_addr, clilen)) < 0)
		 error("ERROR writing (UDP)");
}

void checkBinds(int sockTCP, int sockUDP, int length, struct sockaddr_in serv_addr){
	if (bind(sockTCP, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	if (bind(sockUDP,(struct sockaddr *) &serv_addr, length) < 0)
		error("ERROR on binding");
}

void sendSignalToLog(const char *message, int logportno) {
	int socklog, k;
	socklen_t length;
	struct sockaddr_in logserver;

	struct hostent *server = gethostbyname("localhost");
	if(server == NULL) {
		error("ERROR, host not found\n");
	}

	logserver.sin_family = AF_INET;
	logserver.sin_port = htons(logportno);
	length = sizeof(struct sockaddr_in);

	socklog = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket to log_s
	if(socklog < 0) {
		error("\nsendSignalToLog: ERROR creating log socket\n");
	}
	else {
		printf("\nConnecting to log_s. Port: %d\n", logportno);
		k = sendto(socklog, message, strlen(message), 0, (const struct sockaddr *)&logserver, length);
		if (k < 0) {
			error("\nERROR sending to log_s\n");
		}
		close(socklog);
	}
}

void handleSignal(int sig, siginfo_t *si, void *context)
{
   const char* message = "echo_s is closing";
   int portno = getlogportno();
   
   if (sig == SIGINT)
   {
         printf("\nSIGINT. Control-C was pressed: mypid = %d, mypgid = %d\n",
            getpid(), getpgid(getpid()));
	sendSignalToLog(message, portno);
	exit(0);
   }
}
   
