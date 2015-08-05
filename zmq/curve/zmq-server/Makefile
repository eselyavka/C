CC=gcc
CFLAGS=-c -Wall
LIBS=-lzmq
INCLUDES=-I/usr/local/include 
INCLUDE_LIBS=-L/usr/local/lib


all: server

server: server.o
	$(CC) $(LIBS) $(INCLUDE_LIBS) server.o -o server

server.o: server.c
	$(CC) $(CFLAGS) $(INCLUDES) server.c

clean:
	rm -f *.o *.core server
