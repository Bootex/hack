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
		

			tcph = (struct tcphdr *) (rbuf+20);

			printf(" [TCP] --------------------------------\n");
			printf("  Source Port     : %u\n", ntohs(tcph-> source));
			
			printf(" [RAW] -------------------------------\n");
			
			printf("IPv4 protocol PF_INET : %d \n",PF_INET);
			printf("IPv4 addres AF_INET : %d \n",AF_INET);
			printf("Version %d \n",iph->version);

			printf("tcp source %d\n",ntohs(tcph->source));
			printf("tcp des %d\n",ntohs(tcph->dest));

                        for (i=0; i<50; i++) {
                        printf("[%02x]", rbuf[i]);
                        if ((i+1) % 16==0) printf("\n");
                        }

		
		}	
	}
}
