#My make file for serial port
serial: serial.o
	gcc -o serial serial.o
serial.o: serial.c
	gcc -c serial.c
clean: 
	rm -rf *.o *.core serial
