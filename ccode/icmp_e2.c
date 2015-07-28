#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/ip.h>
#include <linux/icmp.h>

#define TARGET_IP "8.8.8.8"
#define MAXCOUNT 65535

unsigned short in_cksum(u_short *addr, int len)
{

    int         sum=0;
    int         nleft=len;
    u_short     *w=addr;
    u_short     answer=0;

    while (nleft > 1){
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1){
        *(u_char *)(&answer) = *(u_char *)w ;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return(answer);

}

int main(int argc, char **argv)
{

        unsigned char packet[sizeof(struct icmphdr)+MAXCOUNT];
        int raw_socket;
	int COUNT;

        struct iphdr *iphdr;
        struct icmphdr *icmphdr;
        struct sockaddr_in address;
	struct pseudohdr *pseudo_header;

	if (argc==1) COUNT=0;
	else COUNT = atoi(argv[1]);
        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );


	icmphdr = (struct icmphdr *) packet;

	icmphdr->type = ICMP_ECHO;
	icmphdr->un.echo.id = 65535;
	icmphdr->un.echo.sequence= 65535;
	memset(&packet[sizeof(struct icmphdr)],'A',COUNT);
    
     	icmphdr->checksum=in_cksum((u_short *)icmphdr, sizeof(struct icmphdr)+COUNT);
        address.sin_family = AF_INET;
        address.sin_port = htons(0);
        address.sin_addr.s_addr = inet_addr(TARGET_IP);

	sendto(raw_socket, &packet, sizeof(struct icmphdr)+COUNT, 0x0,
	(struct sockaddr *)&address, sizeof(address));
}
