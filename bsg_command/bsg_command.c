#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define MAXPORTNUM 65535
#define HEADER_LEN 28

typedef struct _tcp_packet {
	long  sinchro;
	long  len;
	long  cid;
	long  id;
	time_t  time;
	long chan;
	long code;
	char  buf[512-HEADER_LEN];
} TCP_PACKET;

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char *filename;
    char msisdn[12];
    char code[5];
    char command[1001];
    
    TCP_PACKET proto; 

    read_xml(&msisdn,&code,&command);
    
    proto.sinchro=0xFEFEFEFE;
    proto.cid=0;
    proto.id=0;
    proto.chan=4;
    proto.time=time(NULL);

    proto.code = atoi(code);
    strcpy(proto.buf, msisdn);
    strcat(proto.buf, ";");
    strcat(proto.buf, command);
    //printf("%s\n", proto.buf);
    //printf("%d\n", strlen(proto.buf));
    proto.len=sizeof(proto.sinchro)+sizeof(proto.cid)+sizeof(proto.id)+sizeof(proto.time)+sizeof(proto.chan)+sizeof(proto.code)+strlen(proto.buf)+1;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port confile\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
	
    if (portno > MAXPORTNUM)
    {
		fprintf(stdout, "ERROR portnumber is bigger then 65535\n");
		exit(0);
    }

    sockfd = socket(PF_INET, SOCK_STREAM, 6);
    if (sockfd < 0) 
    {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) 
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
    { 
        error("ERROR connecting");
    }
    else
    {
	printf("Connected!\n");
    }	

    n = write(sockfd,&proto,sizeof(proto));
    if (n < 0)
    { 
        error("ERROR writing to socket");
    }
    else
    {
		printf("Write into socket\n");
    }

	printf("%s\n",proto.buf);
	
    bzero(&proto,sizeof(proto));
    n = read(sockfd,&proto,sizeof(proto));

    if (n < 0)
    { 
        error("ERROR reading from socket");
    }
    else
    {
		printf("Read from socket\n");
    }

    printf("%s\n",proto.buf);

    return 0;
}


