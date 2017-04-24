/*
header file for server_functions.c
*/

void error(const char *msg);
void dostuffTCP(int sockfd);
void dostuffUDP(int sockfd2, socklen_t clilen, struct sockaddr_in cli_addr);
void checkBinds(int sockfd, int sockfd2, int length, struct sockaddr_in serv_addr);
