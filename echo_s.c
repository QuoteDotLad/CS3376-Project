/*file: echo_s.c
**authors: Chance Ball, Rodolfo Galarza, Stephen Mercado, Brian Nguyen, Jimmy Nhes
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

#include <arpa/inet.h>
#include <signal.h>
#include "server_functions.h"

int main(int argc, char *argv[])
{
	int sockTCP, sockUDP, length, newsockfd, portno, logportno, pid, pid2;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr, log_addr;
	fd_set sockset;

	//checks to see if user passes a port number argument.
	//if not, then an error mesage is displayed
	if (argc < 2){
	   fprintf(stderr,"ERROR, no port provided.\n");
	   exit(1);
	}
	
	if(argc > 8) {
	  fprintf(stderr, "ERROR, too many arguments.\n");
	  printf("Usage: %s <port1> [<port2> <port3>] [-logip <IP> -logport <port>]", argv[0]);
	  exit(1);
	}
	
	int sockLog = socket(AF_INET, SOCK_DGRAM, 0);
	
	length = sizeof(serv_addr);
	
	log_addr.sin_family = AF_INET;
	log_addr.sin_addr.s_addr = INADDR_ANY;
	int i, logPort = 9999;
	
	//for loop checks for either option (-logip or -logport) and then sets appropriate value to argument(s) passed.
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i],"-logip") == 0){
			log_addr.sin_addr.s_addr = inet_addr(argv[i+1]); //Copy given IP into logIP
			break;
		}
	}
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i],"-logport") == 0){
			log_addr.sin_port = htons(atoi(argv[i+1]));
			logportno = atoi(argv[i+1]);
			break;
		}
	}
	
	if(connect(sockLog, (struct sockaddr*)&log_addr, sizeof(log_addr)) < 0) {
		error("Unable to connect to log server");
	}

	
	//create child process
	pid = fork();
	
	//These if statements handle the multiple arguments
	//echo_s <port1> [<port2> <port3>]
	
	if(pid != 0){
		pid2 = fork();	//creation of second child process to handle other arguments
	}
	
	if(pid == 0){
		if(argc >= 7){
			sockTCP = socket(AF_INET, SOCK_STREAM, 0); //creation of TCP socket
			sockUDP = socket(AF_INET, SOCK_DGRAM, 0); //creation of UDP socket
			
			bzero((char *) &serv_addr, sizeof(serv_addr)); //set values in buffer to zero
			
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = INADDR_ANY;
			
			portno = atoi(argv[2]);	 //second argument
			
			serv_addr.sin_port = htons(portno);
			
			//check to see if binds are successful
			checkBinds(sockTCP, sockUDP, length, serv_addr);
		}
		if(argc < 7){ //if in child process and only 1 port number provided
			exit(0);
		}
	}
	else if(pid2 == 0){
		if(argc == 8){//if in 2nd child process; handles 3rd port number (if provided)
			sockTCP = socket(AF_INET, SOCK_STREAM, 0); //creation of TCP socket
			sockUDP = socket(AF_INET, SOCK_DGRAM, 0); //creation of UDP socket
			
			bzero((char *) &serv_addr, sizeof(serv_addr)); //set values in buffer to zero
			
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = INADDR_ANY;
			
			portno = atoi(argv[3]);
			
			serv_addr.sin_port = htons(portno);
			
			//check to see if bind is successful
			checkBinds(sockTCP, sockUDP, length, serv_addr);
		}
		if(argc < 8){ //if in 2nd child process; handles 2nd port number (if provided)
			exit(0);
		}
	} else{
			sockTCP = socket(AF_INET, SOCK_STREAM, 0); //creation of TCP socket
			sockUDP = socket(AF_INET, SOCK_DGRAM, 0); //creation of UDP socket
			
			bzero((char *) &serv_addr, sizeof(serv_addr)); //set values in buffer to zero
			
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = INADDR_ANY;
			
			portno = atoi(argv[1]);	 //second argument
			
			serv_addr.sin_port = htons(portno);
			
			//check to see if binds are successful
			checkBinds(sockTCP, sockUDP, length, serv_addr);
	}
	
	//wait for a connection from a client
	listen(sockTCP, 5);
	clilen = sizeof(cli_addr);
	
	setlogportno(logportno);
	
	printf("1. %d\n", getlogportno());
	
	/** This will capture Ctrl-c, only for the parent process **/
	struct sigaction sVal;
	sVal.sa_flags = SA_SIGINFO;		// Signal handler takes 3 arguments, instead of the default, 1.
	sVal.sa_sigaction = handleSignal;	// signal handler
	sigaction(SIGINT, &sVal, NULL);		// Register for SIGINT
	// sigaction(SIGCHLD, &sVal, NULL);	// Register for SIGCHLD

	//continuously creating child processes that perform functions
//	printf("1.%d\n", logportno);
	while (1)
	{
		FD_ZERO(&sockset);
		FD_SET(sockTCP, &sockset);
		FD_SET(sockUDP, &sockset);
		
		int status = select(sockUDP + 1, &sockset, NULL, NULL, NULL);
		
		if(FD_ISSET(sockTCP, &sockset)){
			if(status < 0)
				error("ERROR selecting socket");
			
			newsockfd = accept(sockTCP, (struct sockaddr *) &cli_addr, &clilen);
			
			if (newsockfd < 0)
				error("ERROR on accept");
			
			if ((pid = fork()) < 0)
				error("ERROR on fork");
			
			if (pid == 0){
				close(sockTCP);
				
				//in server_functions
				dostuffTCP(newsockfd, sockLog, cli_addr, log_addr);
				exit(0);
			}
			close(newsockfd);
		}
		
		if(FD_ISSET(sockUDP, &sockset)){
			pid = fork();
			
			if(pid < 0) error("ERROR on fork (UDP)");
			
			if(pid == 0)
			{
				dostuffUDP(sockUDP, sockLog, clilen, cli_addr, log_addr);
				exit(0);
			}
		}
		
		//prevents zombie processes by waiting until all child processes end
		signal(SIGCHLD,SIG_IGN);
	}

	//Close sockets before ending program (doesn't reach here)
	close(sockTCP);
	close(sockUDP);

	return 0;
}