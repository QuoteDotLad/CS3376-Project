CS3376 Semester Project by Group 3

-(1) Chance Ball

-(2) Rodolfo Galarza

-(3) Owais Khan

-(4) Stephen Mercado

-(5) Chanh-Brian Nguyen

-(6) Jimmy Nhes

github repo: https://github.com/QuoteDotLad/CS3376-Project

____________________________________________________________________________________

General Instructions:
Put all the files into one directory and use the command "make" to create the necessary executables
NOTE: using "make clean" will delete the current echo.log file

Server Instructions:
Launch echo_s and log_s. If no IP address or port number is set by the user, both servers will run on default addresses.
NOTE: IP address must be written using "." between numbers.
Using CTRL+C on the echo_s will terminate both echo_s and log_s.

Client Instructions:
Launch either TCP or UDP client and indicate the host and port number (ex. ./ client_tcp cs1 55555)
Then type any message and use the ENTER KEY to send.

____________________________________________________________________________________

First Deliverable
(1) Chance Ball
- edited basic server code to be able to accept both TCP and UDP connections
(2) Rodolfo Galarza
- edited basic server code to make it run continuously that created child processes
- created Readme file
(3) Owais Khan
- edited basic server code to be able to accept both TCP and UDP connections
(4) Stephen Mercado
- edited basic server code to make it run continuously that created child processes
(5) Chanh-Brian Nguyen
- edited basic server code to handle zombie processes
- created server_functions.c and client_functions.c files
(6) Jimmy Nhes
- edited basic server code to handle zombie processes
- created Makefile

Second Deliverable
(1) Chance Ball
- modified Makefile
- modified Readme
- formatted echo.log messages
(2) Rodolfo Galarza
- modified echo_s to work with multiple ports 
(3) Owais Khan
- modified echo_s
(4) Stephen Mercado
- modified echo_s
(5) Chanh-Brian Nguyen
- modified echo_s to work with multiple ports
- created both client_tcp.c and client_udp.c
(6) Jimmy Nhes
- created log_s
- helped with TCP and UDP connections in echo_s

Third Deliverable
(1) Chance Ball
- created and uploaded the project files to github
- modified log server to indicate what IP address the log server is to run at
(2) Rodolfo Galarza
- modified "echo_s" to indicate what port address the log server is to run at
(3) Owais Khan
- modified "log_s" to add an additional argument to indicate what port address to listen on
(4) Stephen Mercado
- modified both "echo_s" and "log_s" so that a stopping message will appear when "echo_s" is stopped with CTRL+C
(5) Chanh-Brain Nguyen
- modify "log_s" to log a stopping message when it recieves from the server and then terminate itself
(6) Jimmy Nhes
- modified Readme to document what each user did
