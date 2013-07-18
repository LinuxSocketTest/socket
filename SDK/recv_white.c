#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>

//#define MAC_ADDR_LEN     6

int main(int argc, char **argv)
{
	int s;
	struct sockaddr_ll src_addr;
	char packet[1400];

	
	//MacAddress src_mac = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};
	
	//memcpy(&(src_addr.sll_addr), src_mac, MAC_ADDR_LEN);

	if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}

	src_addr.sll_family = htons(AF_PACKET);
	src_addr.sll_protocol = htons(ETH_P_ALL);
	src_addr.sll_halen = 6;
	src_addr.sll_ifindex = 2;

	memset(packet, 0, sizeof(packet));
	socklen_t *len = (socklen_t *)sizeof(src_addr);
	unsigned int fromlen = sizeof(src_addr);

	printf("bfr recev \n");
	while(1) {
		printf("aftr recev \n");
		if (recvfrom(s, &packet, sizeof(packet), 0,
					(struct sockaddr *)&src_addr, &fromlen) < 0)
			perror("uh oh:");

		//int i = sizeof(struct iphdr); /* print the payload */
		int i = 0; /* print the payload */
//		for(; i < sizeof(packet); i++) {
//			printf("%c", packet[i]);
//		}
//
			printf("%s", packet);
		printf("\n");
	}
	return(0);
}
