mvd: ini.o mvd.o open_read_cfg.o mysql.o
	gcc -W `pcre-config --libs` -L/usr/local/lib/mysql -lmysqlclient -o mvd ini.o open_read_cfg.o mysql.o mvd.o 
ini.o: ini.c ini.h
	gcc -c ini.c
open_read_cfg.o: open_read_cfg.c
	gcc -W `pcre-config --libs` -c open_read_cfg.c
mysql.o: mysql.c
	gcc -L/usr/local/lib/mysql -lmysqlclient -c mysql.c
mvd.o: mvd.c
	gcc -W `pcre-config --libs` -c mvd.c
install:
	cp ./mvd.conf /usr/local/etc/mvd	
clean: 
	rm -f *.o *.core mvd

