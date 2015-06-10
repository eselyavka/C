#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

unsigned short in_cksum(unsigned short *addr, int len)
{
        int nleft = len;
        int sum = 0;
        unsigned short *w = addr;
        unsigned short answer = 0;

        while (nleft > 1) {
                sum += *w++;
                nleft -= 2;
        }

        if (nleft == 1) {
                *(unsigned char *) (&answer) = *(unsigned char *) w;
                sum += answer;
        }
        
        sum = (sum >> 16) + (sum & 0xFFFF);
        sum += (sum >> 16);
        answer = ~sum;
        return (answer);
}

int main() {
        int s;
        struct ip ip;
        struct icmp icmp;
        struct sockaddr_in sin;
        int on=1;
        char *sdata="eselyavka";
        u_char *packet;

        packet=(u_char *) malloc(60);

        ip.ip_v = 0x4;
        ip.ip_hl = 0x5;
        ip.ip_tos = 0x0;
        ip.ip_len = 0x003C;
        ip.ip_id = 0x0100;
        ip.ip_off = 0x0;
        ip.ip_ttl = 0x40;
        ip.ip_p = IPPROTO_RAW;
        ip.ip_src.s_addr = inet_addr("192.168.0.1");
        ip.ip_dst.s_addr = inet_addr("192.168.0.1");
        ip.ip_sum = in_cksum((unsigned short *)&ip, sizeof(ip));

        memcpy(packet, &ip, sizeof(ip));

        if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
                perror("raw socket");
                return 1;
        }

        if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
                perror("setsockopt");
                exit(1);
        }

        memcpy(packet + 20, sdata, strlen(sdata));

        memset(&sin, 0, sizeof(sin));

        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr("192.168.0.1");

        if (sendto(s, packet, 60, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
                perror("sendto");
                exit(1);
        }
    return 0;
}
