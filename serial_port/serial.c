#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
/*
 * * 'open_port()' − Open serial port 1.
 * *
 * * Returns the file descriptor on success or −1 on error.
 * */
int open_port(void)
{
	int fd; 
	int colr;
	char ch[10];
	fd = open("/dev/cuad1", O_RDWR | O_NOCTTY | O_NDELAY);
	printf("%d\n",fd);
	if (fd == -1)
	{
		perror("open_port: Unable to open /dev/cuad1 − ");
		return 1;
	}
	else
	{
		//fcntl(fd, F_SETFL, 0);
		read(fd,ch,10);
		close(fd);
		printf("%s\n", ch);
		return 0;
	}
}

int main(void)
{
	open_port();
	return 0;
}
