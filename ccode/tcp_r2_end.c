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

#define MAXCOUNT 65536 

int main()
{

        int raw_socket;

        struct sockaddr address;
	struct iphdr *iph ;
	struct tcphdr *tcph;
	struct sockaddr_in src, dest;

	int size;
	unsigned char rbuf[MAXCOUNT];
	int s,i;
	int iphdrlen;
	int count=0;
	
        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_TCP );
	if (raw_socket <0) perror("socket");

	memset(rbuf, 0, sizeof(rbuf));
	while (1) 
	{
		s=recvfrom(raw_socket, rbuf, MAXCOUNT,0, &address, &size);
		if (s<0) perror("recvfrom");
        	else
		{
			printf("\n ======================================");
			printf("\n DEBUG = %d", ++count);

			iph = (struct iphdr *) rbuf;
			iphdrlen = iph-> ihl *4;
			src.sin_addr.s_addr = iph->saddr;
			dest.sin_addr.s_addr = iph->daddr;
		
			printf("\n [IP] ---------------------------------");
			printf("\n");
			printf("  IP Version      : %d\n", iph->version);
			printf("  Header Length   : %d\n", iphdrlen);
			printf("  IP Total Length : %d\n", ntohs(iph->tot_len));
			printf("  Source IP       : %s\n", inet_ntoa(src.sin_addr));
			printf("  Destination IP  : %s\n", inet_ntoa(dest.sin_addr)); 
			printf("  Protocol	 : %d\n", iph-> protocol);
			printf("  TTL		 : %d\n", iph-> ttl);


			tcph = (struct tcphdr *) (rbuf+20);

			printf(" [TCP] --------------------------------\n");
			printf("  Source Port     : %u\n", ntohs(tcph-> source));
			printf("  Destination Port: %u\n", ntohs(tcph->dest));
			printf("  Sequence Number : %u\n", ntohl(tcph->seq));
			printf("  Acknowldege No. : %u\n", ntohl(tcph->ack_seq));
			printf("  Flags 		 :");
			if (tcph->urg) printf(" URG");
			if (tcph->ack) printf(" ACK");
			if (tcph->psh) printf(" PSH");
			if (tcph->rst) printf(" RST");
			if (tcph->syn) printf(" SYN");
			if (tcph->fin) printf(" FIN");
			printf("\n");	
			
			printf(" [RAW] -------------------------------\n");

                        for (i=0; i<50; i++) {
                        printf("[%02x]", rbuf[i]);
                        if ((i+1) % 16==0) printf("\n");
                        }

		
		}	
	}
}
