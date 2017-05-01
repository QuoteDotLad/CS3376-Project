/*
header file for server_functions.c
*/

void error(const char *msg);
void dostuffTCP(int sockTCP, int sockLog, struct sockaddr_in cli_addr, struct sockaddr_in log_addr);
void dostuffUDP(int sockUDP, int sockLog, socklen_t clilen, struct sockaddr_in cli_addr, struct sockaddr_in log_addr);
void checkBinds(int sockfd, int sockfd2, int length, struct sockaddr_in serv_addr);
void sendSignalToLog(const char *message, int logportno);
void handleSignal(int sig, siginfo_t *si, void *context);
void setlogportno(int lp);
int getlogportno();