/* UDP client in the internet domain */

/*file: client_udp.c
**authors: Stephen Mercado, Brian Nguyen, Jimmy Nhes
**date: 4/18/17
**description: this is the source code for client_funtions the first delieverable. This file will contain any functions that are relevant and useful to the client.c code and will help with elegance.
**NOTE: The code that we wrote is a modification on the code that can be found at the following site:  http://www.linuxhowtos.org/C_C++/socket.htm
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "client_functions.h"

int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
   
   checkArgs(argc);
   
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   char *findnull;
	if((findnull = strchr(buffer, '\n')) != NULL) *findnull = '\0';
	else error("ERROR removing new line character"); 
   n=sendto(sock, buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
			
   if (n < 0) error("sendto");
   
   n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
   
   if (n < 0) error("recvfrom");
   
   write(1,buffer,n);
   printf("\n");
   close(sock);
   return 0;
}
