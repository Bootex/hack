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
#define SPOOFED_IP "192.168.32.107"

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

int main()
{

        unsigned char packet[28];
        int raw_socket;
        int on = 1 ;

        struct iphdr *iphdr;
	struct icmphdr *icmphdr;
        struct sockaddr_in address;
        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_RAW );
        setsockopt( raw_socket, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on) );

	icmphdr = (struct icmphdr *) (packet + 20);
	memset((char *)icmphdr, 0,12);
	icmphdr->type = ICMP_ECHO;
        icmphdr->un.echo.id = 65535;
        icmphdr->un.echo.sequence= 65535;
        icmphdr->checksum=in_cksum((u_short *)icmphdr, sizeof(struct icmphdr));
	
	/* deleted */
	
        iphdr->saddr = inet_addr(SPOOFED_IP);
        iphdr->daddr = inet_addr(TARGET_IP);
	iphdr->check = in_cksum((u_short *)iphdr, sizeof(struct iphdr));


        address.sin_family = AF_INET;
        address.sin_port = htons(0);
        address.sin_addr.s_addr = inet_addr(TARGET_IP);
	sendto( raw_socket, &packet, sizeof(packet), 0x0,
	(struct sockaddr *)&address, sizeof(address));
}
