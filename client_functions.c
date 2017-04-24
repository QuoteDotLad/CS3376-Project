/*file: client_functions.c
**authors: Brian Nguyen, Jimmy Nhes
**date: 4/14/17
**description: this is the source code for client_funtions the first delieverable. This file will contain any functions that are relevant and useful to the client.c code and will help with elegance.
**NOTE: The code that we wrote is a modification on the code that can be found at the following site:  http://www.linuxhowtos.org/C_C++/socket.htm
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdbool.h>
#include <signal.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//checks to see if user inputted correct number of arguments
 void checkArgs(int noargs){
	if(noargs != 3){
		fprintf(stderr,"usage %s <hostname> <port>\n");
		exit(1);
	}
		
}