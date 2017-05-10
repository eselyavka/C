#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG_SIZE 3

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     int buf_size = 256;
     char buffer[buf_size];
     struct sockaddr_in serv_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     memset(&serv_addr, '\0', sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd, BACKLOG_SIZE);
     while (1) {
        struct sockaddr_in cli_addr;
        socklen_t client;
        client = sizeof(cli_addr);
        newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &client);

        if (newsockfd < 0) 
              error("ERROR on accept");
         memset(buffer, '\0', buf_size);

        n = read(newsockfd,buffer,buf_size - 1);

        if (n < 0) error("ERROR reading from socket");

        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);

        if (n < 0) error("ERROR writing to socket");

        close(newsockfd);
     }
     close(sockfd);
     return 0;
}
