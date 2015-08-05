CC=gcc
CFLAGS=-c -Wall
LIBS=-lzmq
INCLUDES=-I/usr/local/include 
INCLUDE_LIBS=-L/usr/local/lib


all: client

client: client.o
	$(CC) $(LIBS) $(INCLUDE_LIBS) client.o -o client

client.o: client.c
	$(CC) $(CFLAGS) $(INCLUDES) client.c

clean:
	rm -f *.o *.core client
