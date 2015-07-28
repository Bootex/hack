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

#define TARGET_IP "http://kitribob.kr"

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
	int input_size;

        unsigned char packet[sizeof(struct icmphdr)];
        int raw_socket;

        struct iphdr *iphdr;
        struct icmphdr *icmphdr;
        struct sockaddr_in address;

	scanf("%d",&input_size);

        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );


	icmphdr = (struct icmphdr *) packet;

	icmphdr->type = ICMP_ECHO;
	icmphdr->un.echo.id = 65535;
	icmphdr->un.echo.sequence= 65535;
        icmphdr->checksum=in_cksum((u_short *)icmphdr, sizeof(struct icmphdr));
    
while(input_size){
        address.sin_family = AF_INET;
        address.sin_port = htons(0);
        address.sin_addr.s_addr = inet_addr(TARGET_IP);
	printf("\n Debug = %d", sizeof(struct icmphdr));
	sendto(raw_socket, &packet, sizeof(struct icmphdr), 0x0,
	(struct sockaddr *)&address, sizeof(address));

	input_size--;
}
}
