/*
header file for server_functions.c
*/

void error(const char *msg);
void dostuffTCP(int sockfd, struct sockaddr_in cli_addr, const char *logIP/*, int argc, char **argv*/);
void dostuffUDP(int sockfd2, socklen_t clilen, struct sockaddr_in cli_addr, const char *logIP);
void checkBinds(int sockfd, int sockfd2, int length, struct sockaddr_in serv_addr);
const char *getIP(struct sockaddr_in *cliAddr);
void logsend(const char *buffer, char *ipAddr, const char *logIP);
