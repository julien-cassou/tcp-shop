CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: udp tcp

udp: serveur_udp client_udp

server_udp: serveur_udp.c
	$(CC) $(CFLAGS) -o serveur_udp serveur_udp.c
client_udp: client_udp.c
	$(CC) $(CFLAGS) -o client_udp client_udp.c

tcp: serveur_tcp client_tcp

server_tcp: serveur_tcp.c
	$(CC) $(CFLAGS) -o serveur_tcp serveur_tcp.c
client_tcp: client_tcp.c
	$(CC) $(CFLAGS) -o client_tcp client_tcp.c

clean:
	rm -f serveur_udp client_udp serveur_tcp client_tcp