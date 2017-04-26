/*file: server_functions.c
**authors: Brian Nguyen
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
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>

 void error(const char *msg) //Prints an error on system call from stderr
{
	perror(msg);
	exit(1);
}

const void *getIP(struct sockaddr_in cliAddr, char * ipBuf) { //gets IP address of client specified in structure

	inet_ntop(AF_INET,&cliAddr.sin_addr.s_addr, ipBuf, INET_ADDRSTRLEN); 	

}

void logsend(const char *buffer, const char *ipAddr, const char *logIP) { //Receives client IP, message they send, and 
	int socklog, k, portno;
	socklen_t length;
	struct sockaddr_in logserver;
	portno = 9999; //Default, expected to be changed in code from user-2
	struct hostent *server = gethostbyname(logIP); //Gets the log server IP and fills hostent's fields
	if(server == NULL){error("ERROR, host not found\n");}

	//bzero((char *)&logserver, sizeof(logserver));
	logserver.sin_family = AF_INET;
	/*bcopy((char *)server->h_addr_list[0], //An array of addresses in hostent struct, #Def at bottom of it
      	      (char *)&logserver.sin_addr.s_addr, 
       	server->h_length);*/
	//logserver.sin_addr.s_addr = INADDR_ANY/*inet_addr(logIP)*/;	
	logserver.sin_port = htons(portno);
		
	char logmsg[256] = {0};
	length = sizeof(struct sockaddr_in);
		
	strcat(logmsg, buffer);
	strcat(logmsg, " received from: "); 
	strcat(logmsg, ipAddr);

	socklog = socket(AF_INET, SOCK_DGRAM, 0); //Creating log's socket
	if(socklog < 0){error("ERROR creating log socket");}
	

	k = sendto(socklog, logmsg, strlen(logmsg), 0, (const struct sockaddr *)&logserver, length);
	if(k < 0){error("ERROR sending to log_s");} //Sending the buffer to log_s

}

void dostuffTCP(int sockfd, struct sockaddr_in cli_addr, const char *logIP/*, int argc, char **argv*/){
	int n/*, k, length, socklog, portno*/;
	char buffer[256];

	//change to 9999 upon submission
	//portno = 9999; //move to func
      
	bzero(buffer,256);
      
	if ((n = read(sockfd,buffer,255)) < 0)
		error("ERROR reading from socket");
	char ipBuf[INET_ADDRSTRLEN];	
	const char *ipAddr = (char *)(getIP(cli_addr, ipBuf));
	
	logsend(buffer, ipAddr, logIP);	

	//creating a socket that will be used to connect to log_s
	//socklog = socket(AF_INET, SOCK_DGRAM, 0);
	//if (socklog < 0) error("ERROR creating socket");
	 
	//logserver.sin_family = AF_INET;
	/*int i = 0;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-logip") == 0) {
		logserver.sin_addr.s_addr = inet_addr(argv[i+1]);
		break;
		}
	}*/
	//logserver.sin_addr.s_addr = inet_addr(argv[argc - 1]);
	//logserver.sin_port = htons(portno);
	 
	//length = sizeof(struct sockaddr_in);
	 
	//k = sendto(socklog, buffer, strlen(buffer), 0, (const struct sockaddr *)&logserver, length);
	 
	//if (k < 0) error("ERROR sending to log_s");
	
   
	printf("Your message (TCP): %s", buffer);
   
	if ((n = write(sockfd, buffer, 255)) < 0)
		error("ERROR writing (TCP)");
}

void dostuffUDP(int sockfd2, socklen_t clilen, struct sockaddr_in cli_addr, const char *logIP){
							/*, int argc, char **argv*/
	int n/*, k, length, socklog, portno*/;
	//struct sockaddr_in logserver;
	
    	char buffer[256];
	
	//change to 9999 upon submission
	//portno = 9999;
      
	bzero(buffer,256);
	
	if ((n = recvfrom(sockfd2, buffer, 255, 0, (struct sockaddr *)&cli_addr, &clilen)) < 0)
		error("ERROR reading (UDP)");
	
	char ipBuf[INET_ADDRSTRLEN];
	const char *ipAddr = (char *)(getIP(cli_addr, ipBuf)); //Getting IP address of client
	
	logsend(buffer, ipAddr, logIP); //Sending the message and IP to log

	//creating a socket that will be used to connect to log_s
	//socklog = socket(AF_INET, SOCK_DGRAM, 0);
	//if (socklog < 0) error("ERROR creating socket");
	 
	/*logserver.sin_family = AF_INET;
	int i = 0;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-logip") == 0){
		logserver.sin_addr.s_addr = inet_addr(argv[i+1]);
		break;
		}
	}*/
	
	
	printf("Your message (UDP): %s", buffer);
	
	if ((n = sendto(sockfd2, buffer, 255, 0 , (struct sockaddr *)&cli_addr, clilen)) < 0)
		 error("ERROR writing (UDP)");
}

void checkBinds(int sockfd, int sockfd2, int length, struct sockaddr_in serv_addr){
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	if (bind(sockfd2,(struct sockaddr *) &serv_addr, length) < 0)
		error("ERROR on binding");
}


