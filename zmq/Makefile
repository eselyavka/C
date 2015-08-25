CC=gcc
CFLAGS=-c -Wall
LIBS=-lzmq -lpthread


all: zmq-server zmq-client

zmq-client: zmq-client.o
	$(CC) $(LIBS) zmq-client.o -o zmq-client

zmq-client.o: zmq-client.c
	$(CC) $(CFLAGS) $(LIBS) zmq-client.c

zmq-mt-client: zmq-mt-client.o
	$(CC) $(LIBS) zmq-mt-client.o -o zmq-mt-client

zmq-mt-client.o: zmq-mt-client.c
	$(CC) $(CFLAGS) $(LIBS) zmq-mt-client.c

zmq-server: zmq-server.o
	$(CC) $(LIBS) zmq-server.o -o zmq-server

zmq-server.o: zmq-server.c
	$(CC) $(CFLAGS) $(LIBS) zmq-server.c

clean:
	rm -f *.o *.core zmq-server zmq-client
