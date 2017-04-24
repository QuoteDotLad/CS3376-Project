Group 3:  
Members: Brian Nguyen, Chance Ball, Jimmy Nhes, Owais Khan, Rodolfo Galaraza, Stephen Mercado


Second Deliverable:
	The code that we wrote is a modification on the code submitted for the first deliverable
	For the second deliverable we decided to split our group into three subgroups of two people per subgroup.
	
	Files:
	client_tcp.c		-Client code handling TCP connections
	client_udp.c		-Client code handling UDP connections
	client_functions.c	-Client code functions
	client_functions.h	-Client functions header file
	echo_s.c		-Echo server code
	server_functions.c	-Server code functions
	server_functions.h	-Server functions header file
	log_s.c			-Server that logs messages into echo.log
	Makefile		-Makes all executables at once. WARNING: using "make clear" will delete the echo.log file
	README			-This file
		
	Members and what they've done in Deliverable 2:
	Chance Ball: Created Makefile, and README, Formatting of echo.log messages and minor adjustments to log_s
	Rodolfo Galaraza: Made echo_s work with multiple ports
	Owais Khan: Worked on echo_s
	Stephen Mercado: Worked on echo_s
	Brian Nguyen: Created both  client_tcp.c and client_udp.c; Made echo_s work with multiple ports
	Jimmy Nhes: Created log_s; helped with functionality on echo_s