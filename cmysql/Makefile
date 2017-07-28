mysql: mysql.o test.o  
	gcc -L/usr/local/lib/mysql -lmysqlclient -o mysql mysql.o test.o
mysql.o: mysql.c
	gcc -L/usr/local/lib/mysql/ -c mysql.c
test.o: test.c
	gcc -c test.c
clean:	
	rm -f *.o *.core mysql
