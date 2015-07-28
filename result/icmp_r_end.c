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

#define MAXCOUNT 65535 

int main()
{

        int raw_socket;

        struct sockaddr address;
	int size;
	unsigned char rbuf[MAXCOUNT];
	int s,i;
	int count=0;
	
        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );
	if (raw_socket <0) perror("socket");

	memset(rbuf, 0, sizeof(rbuf));
	while (1) {
	s=recvfrom(raw_socket, rbuf, MAXCOUNT,0, &address, &size);
	if (s<0) perror("recvfrom");
        else
	{
		printf("\n ===============================");
		printf("\n DEBUG = %d", ++count);

		printf("\n size = %d %d\n", size,s );
		for (i=0; i<50; i++) {
			printf("[%02x]", rbuf[i]);
			if ((i+1) % 16==0) printf("\n");
		}
	}
	}
}
