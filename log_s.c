// Log Server
// Receives messages from the echo server
// and prints out the messages into a log file
// By: Jimmy Nhes
// Formatting: Chance Ball
// Additional code: Owais Khan
// description:- Modified log_s so that additional arg passed //indicates port num to listen for.

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include "server_functions.h"


int main(int argc, char *argv[])
{

	int sock, length, n;
   
// portno should be "9999" upon submission; use "55556" for testing purposes
	//int portno = 9999;
	int portno;
	socklen_t fromlen;// length of server address
	struct sockaddr_in server;   
	struct sockaddr_in from;
	char buf[1024];
	char s[1024];

// file pointer needed to write to a log text file   
	FILE *fp;

// array used to hold the port numbers
	char portNum[128];  
	int num = 0;

// Error if no port number is provided...   
// Port Number should be hard coded as "9999" for project purposes
// Using Port Number "55556" for testing purposes

// Checks to see if port number passed as arguement
// if not, error
	if (argc < 2) {
	//if(argc == 0){ - similar to what is in log_s2 - if no arg given	
		//fprintf(stderr, "ERROR, no port provided\n");
		fprintf(stderr, "ERROR, no port provided\nUsing default port number");
		portno = 9999;
		//exit(0);  
	}
	//if using the check if no arg given, no need for else statement
	else if(argc > 6){
		//fprintf(stderr, "Too many arguements.\nUsing default port number");
		fprintf(stderr, "Too many arguments.\nUsing default port number"); 
		portno = 9999;
		//exit(1);   
	}
	else{ // Sets arg passed in as port number.
		int i;
		for(i = 0; i < argc; i++){
			if(strcmp(argv[i], "-logip") == 0 ){
				break;	
			}
			else{
				strcpy(portNum[i], argv[i]); 
				num++;
			}
		}
		//alternative in case the for loop causes issues.
		/*
			portno = argv[1];
			//I believe this will set the passed arg to the portno to be used.
		*/
	}
	
	// convert the string into an int to save the port number
	// not necessary for the alternative
	int portno = (int)portNum;
	
	sock=socket(AF_INET, SOCK_DGRAM, 0);    
	
	if (sock < 0) 
		error("Opening socket");
   
	length = sizeof(server);
	bzero(&server,length); // set values in buffer to 0
	server.sin_family=AF_INET; 
	server.sin_addr.s_addr=INADDR_ANY; 
   
// changing server.sin_port=htons(atoi(argv[1])) into 
//htons(portno)
	server.sin_port=htons(portno);
	
	// network byte conversion
	//server.sin_port = htons(port);
	
// Compare passed in port number to actual port number.
	// not necessary for alternative.
	if(port != portno)
		error("Incorrect port number");
	
// No more changes after this.
   	if (bind(sock,(struct sockaddr *)&server,length)<0)
		error("binding");
   	fromlen = sizeof(struct sockaddr_in);
   	while (1) {
       	n = recvfrom(sock,buf,1024,0,(struct sockaddr*)&from,&fromlen);
       if (n < 0) 
		error("recvfrom");
       	
		printf("\n");
       	write(1,"Received a datagram: ",21);	
       	write(1,buf,n);
      
	   // this line cleans out the fluff in the buffer
	   // so that only the msg is printed out
       	buf[n] = '\0';
	  
	 // open log text file
	   	fp = fopen("echo.log", "a");
	   	if (fp == NULL) {
		   fprintf(stderr, "Can't open output file log.txt");
		   exit(1);
	   	}
	   	time_t t = time(NULL);
           struct tm tm = *localtime(&t);
	   
	   // write to the log text file
	   	fprintf(fp,"%d-%d-%d %d:%d:%d \t %s\n",
	   	tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, 
	   	tm.tm_hour, tm.tm_min, tm.tm_sec, buf
	  /*inet_ntoa(from.sin_addr)*/);
	   // close file
	   	fclose(fp);
	   
       	n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       	if (n  < 0) error("sendto");
   		}
   		return 0;
 	}
