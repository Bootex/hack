#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/icmp.h>

#define MAXCOUNT 50 

int main()
{

        int raw_socket;


        struct sockaddr address;
	int size;
	char rbuf[MAXCOUNT];
	
        raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );
	if (raw_socket <0) perror("socket");
	memset(rbuf, 0, sizeof(rbuf));

	while (1) {
		s=recvfrom(raw_socket, rbuf, MAXCOUNT,0, &address, &size);
		if (s<0) perror("recvfrom");
       		else
		{
			printf("%x",s);

		}
	}
}
