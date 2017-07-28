bsg_command: bsg_command.o readxml.o
	gcc -I/usr/include/libxml2 -Wall -lxml2 -o bsg_command bsg_command.o readxml.o
readxml.o: readxml.c
	gcc -I/usr/include/libxml2 -Wall -lxml2 -c readxml.c
bsg_command.o: bsg_command.c 
	gcc -c bsg_command.c
clean: 
	rm -rf *.o *.core bsg_command 
