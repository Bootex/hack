
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>           // close()
#include <string.h>           // strcpy, memset(), and memcpy()

#include <netdb.h>            // struct addrinfo
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t
#include <sys/socket.h>       // needed for socket()
#include <netinet/in.h>       // IPPROTO_RAW, INET_ADDRSTRLEN
#include <netinet/ip.h>       // IP_MAXPACKET (which is 65535)
#include <arpa/inet.h>        // inet_pton() and inet_ntop()
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl.
#include <net/if.h>           // struct ifreq
#include <linux/if_ether.h>   // ETH_P_ARP = 0x0806
#include <linux/if_packet.h>  // struct sockaddr_ll (see man 7 packet)
#include <net/ethernet.h>

#include <errno.h>            // errno, perror()

// Define a struct for ARP header
struct arphdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};

// Define some constants.
#define ETH_HDRLEN 14      // Ethernet header length
#define IP4_HDRLEN 20      // IPv4 header length
#define ARP_HDRLEN 28      // ARP header length
#define ARPOP_REQUEST 1    // Taken from <linux/if_arp.h>

#define SRC_MAC "\x00\x0c\x29\xea\xf8\x6f"
#define DEST_MAC "\xff\xff\xff\xff\xff\xff"

// Function prototypes
char *allocate_strmem (int);
uint8_t *allocate_ustrmem (int);

int
main (int argc, char **argv)
{
  unsigned char frame[42];
  int i, status, frame_length, sd, bytes;
  char *interface, *target, *src_ip;
  struct arphdr *arphdr;
  uint8_t *src_mac, *dst_mac, *ether_frame;
  struct addrinfo hints, *res;
  struct sockaddr_in *ipv4;
  struct sockaddr_ll device;
  struct ifreq ifr;

  if ((sd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
    perror ("socket() failed to get socket descriptor for using ioctl() ");
    exit (EXIT_FAILURE);
  }


  memset(&ifr, 0, sizeof(ifr));
  strcpy(interface,"eth0");

  snprintf(ifr.ifr_name, sizeof(ifr.ifr_name),"%s", interface);
  if (ioctl(sd, SIOCGIFHWADDR, & ifr) <0) {
	perror("ioctl() failed ");
  }
  close(sd);
  memset(&device, 0, sizeof(device));
  if ((device.sll_ifindex = if_nametoindex(interface)) ==0) {
	perror("if_nametoindex()");
  }

  printf("%s", ifr.ifr_hwaddr.sa_data);
  arphdr = (struct arphdr *) (frame + 14);

  // ARP header
  arphdr->htype = htons (1);
  arphdr->ptype = htons (ETH_P_IP);
  arphdr->hlen = 6;
  arphdr->plen = 4;
  arphdr->opcode = htons (ARPOP_REQUEST);

  memcpy(arphdr-> sender_mac,SRC_MAC,6);

  inet_pton(AF_INET,"172.16.185.190", arphdr->sender_ip);
  inet_pton(AF_INET,"172.16.185.2", arphdr->target_ip);

  memset((char*) arphdr->target_mac, 0, 6);


  memcpy(frame, DEST_MAC, 6);
  memcpy(frame+6, SRC_MAC, 6);
  memcpy(frame+12, "\x08", 1);
  memcpy(frame+13, "\x06", 1);



  printf("\n DEBUG ="); 
  for (i=0;i<42;i++)
	printf("%02x ", frame[i]);

   if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
    perror ("socket() failed ");
   exit (EXIT_FAILURE);
 }

  if ((bytes = sendto (sd, &frame, sizeof(frame), 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
    perror ("sendto() failed");
    exit (EXIT_FAILURE);
  }

  close (sd);


}
