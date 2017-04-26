/*file: echo_s.c
**authors: Chance Ball, Rodolfo Galarza, Owais Khan, Stephen Mercado, Brian Nguyen, Jimmy Nhes
**date: 4/20/17
**description: this is the source code for client_funtions the first delieverable. This file will contain any functions that are relevant and useful to the client.c code and will help with elegance.
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
#include "server_functions.h"



int main(int argc, char *argv[])
{
	int sockfd, sockfd2, length, newsockfd, portno, pid, pid2;
	int numPorts = 0;
	socklen_t clilen; //Length of client address
	struct sockaddr_in serv_addr, cli_addr; //Server and client
	fd_set sockset;
	char logIP[128] = "127.0.0.1"; //Default log_s IP if -logip is not detected
	int ports[3];	

	//checks to see if user passes a port number argument.
	//if not, then an error mesage is displayed
	if (argc < 2){
	   fprintf(stderr,"ERROR, no port provided.\n");
	   exit(1);
	}
	else if(argc > 6) {
	  fprintf(stderr, "ERROR, too many arguments.\n");
	  printf("Usage: %s <port1> [<port2> <port3>] -logip <IP> ",argv[0]);
	  exit(1);
	}
	//else if(isNumber == false)
	int i;
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i],"-logip") == 0){
			strcpy(logIP, argv[i+1]); //Copy given IP into logIP
		break;
		}
		else {
			ports[i-1] = atoi(argv[i]); //Put port #s into array
			numPorts++;
		}
	}
		
	length = sizeof(serv_addr);
	
	//create child process
	pid = fork();
	
	//These if statements handle the multiple arguments
	if(pid != 0){
		pid2 = fork();	//creation of second child process to handle other arguments
	}
	if(argc > 2 && pid == 0){
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //creation of TCP socket
		sockfd2 = socket(AF_INET, SOCK_DGRAM, 0); //creation of UDP socket
		
		bzero((char *) &serv_addr, sizeof(serv_addr)); //set values in buffer to zero
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		
		portno = ports[1]; //second argument
		
		serv_addr.sin_port = htons(portno);
		
		//check to see if binds are successful
		checkBinds(sockfd, sockfd2, length, serv_addr);
	}
	else if(argc < 3 && pid == 0){ //if in child process and only 1 port number provided
		exit(0);
	}
	else if(pid2 == 0 && argc == 4){//if in 2nd child process; handles 3rd port number (if provided)
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //creation of TCP socket
		sockfd2 = socket(AF_INET, SOCK_DGRAM, 0); //creation of UDP socket
		
        bzero((char *) &serv_addr, sizeof(serv_addr)); //set values in buffer to zero
		
	serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
		
        portno = ports[2];
        
        serv_addr.sin_port = htons(portno);
		
		//check to see if bind is successful
		checkBinds(sockfd, sockfd2, length, serv_addr);
	}
	else if(argc < 4 && pid2 == 0){ //if in 2nd child process; handles 2nd port number (if provided)
		exit(0);
	}
	else{
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //creation of TCP socket
		sockfd2 = socket(AF_INET, SOCK_DGRAM, 0); //creation of UDP socket
		
		bzero((char *) &serv_addr, sizeof(serv_addr)); //set values in buffer to zero
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		
		portno = ports[0];
		
		serv_addr.sin_port = htons(portno);
		
		//check to see if bind is successful
		checkBinds(sockfd, sockfd2, length, serv_addr);
	}
	
	//wait for a connection from a client
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	//continuously creating child processes that perform functions
	while (1)
	{
		FD_ZERO(&sockset);
		FD_SET(sockfd, &sockset);
		FD_SET(sockfd2, &sockset);
		
		int status = select(sockfd2 + 1, &sockset, NULL, NULL, NULL);
		
		if(FD_ISSET(sockfd, &sockset)){
			if(status < 0)
				error("ERROR selecting socket");
			
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			
			if (newsockfd < 0)
				error("ERROR on accept");
			
			if ((pid = fork()) < 0)
				error("ERROR on fork");
			
			if (pid == 0){
				close(sockfd);
				
				//in server_functions
				dostuffTCP(newsockfd, cli_addr, logIP/*, argc, argv*/);
				exit(0);
			}
			close(newsockfd);
		}
		
		if(FD_ISSET(sockfd2, &sockset)){
			pid = fork();
			
			if(pid < 0) error("ERROR on fork (UDP)");
			
			if(pid == 0)
			{
				dostuffUDP(sockfd2, clilen, cli_addr, logIP/*, argc, argv*/);
				exit(0);
			}
		}
		
		//prevents zombie processes by waiting until all child processes end
		signal(SIGCHLD,SIG_IGN);
	}

	//Close sockets before ending program (doesn't reach here)
	close(sockfd);
	close(sockfd2);

	return 0;
}
