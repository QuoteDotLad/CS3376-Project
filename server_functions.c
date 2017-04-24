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

 void error(const char *msg) //Prints an error on system call from stderr
{
	perror(msg);
	exit(1);
}

void dostuffTCP(int sockfd){
	int n, k, length, socklog, portno;
	struct sockaddr_in logserver;
	char buffer[256];
	
	//change to 9999 upon submission
	portno = 9999;
      
	bzero(buffer,256);
      
	if ((n = read(sockfd,buffer,255)) < 0)
		error("ERROR reading from socket");
	
	
	//creating a socket that will be used to connect to log_s
	socklog = socket(AF_INET, SOCK_DGRAM, 0);
	if (socklog < 0) error("ERROR creating socket");
	 
	logserver.sin_family = AF_INET;	 
	logserver.sin_port = htons(portno);
	 
	length = sizeof(struct sockaddr_in);
	 
	k = sendto(socklog, buffer, strlen(buffer), 0, (const struct sockaddr *)&logserver, length);
	 
	if (k < 0) error("ERROR sending to log_s");
	
   
	printf("Your message (TCP): %s", buffer);
   
	if ((n = write(sockfd, buffer, 255)) < 0)
		error("ERROR writing (TCP)");
}

void dostuffUDP(int sockfd2, socklen_t clilen, struct sockaddr_in cli_addr){
	int n, k, length, socklog, portno;
	struct sockaddr_in logserver;
	
    char buffer[256];
	
	//change to 9999 upon submission
	portno = 9999;
      
	bzero(buffer,256);
	
	if ((n = recvfrom(sockfd2, buffer, 255, 0, (struct sockaddr *)&cli_addr, &clilen)) < 0)
		error("ERROR reading (UDP)");
	
	
	//creating a socket that will be used to connect to log_s
	socklog = socket(AF_INET, SOCK_DGRAM, 0);
	if (socklog < 0) error("ERROR creating socket");
	 
	logserver.sin_family = AF_INET;	 
	logserver.sin_port = htons(portno);
	 
	length = sizeof(struct sockaddr_in);
	 
	k = sendto(socklog, buffer, strlen(buffer), 0, (const struct sockaddr *)&logserver, length);
	 
	if (k < 0) error("ERROR sending to log_s");
	
	
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
