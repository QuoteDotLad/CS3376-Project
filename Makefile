echo_project: echo_s log_s client_tcp client_udp

client_tcp: client_tcp.o client_functions.o
	g++ -o client_tcp client_tcp.c client_functions.c

client_udp: client_udp.o client_functions.o
	g++ -o client_udp client_udp.c client_functions.c

echo_s: echo_s.o server_functions.o
	g++ -o echo_s echo_s.c server_functions.c

log_s: log_s.o
	g++ -o log_s log_s.c

clean:
	rm *.o echo_s log_s client_tcp client_udp echo.log
