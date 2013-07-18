#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/ether.h>


int main(int argc, char **argv)
{
	int s;
	int i = 0;
	struct sockaddr_ll src_addr;
	char packet[1400];

	if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}

	memset(packet, 0, sizeof(packet));
	socklen_t *len = (socklen_t *)sizeof(src_addr);
	int fromlen = sizeof(src_addr);

	while(1) {
		if (recvfrom(s, &packet, sizeof(packet), 0,
					(struct sockaddr *)&src_addr, &fromlen) < 0)
			perror("uh oh:");
		
		printf("\n");
	}
	return(0);
}
