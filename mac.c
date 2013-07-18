#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/ether.h>

#define ETHERTYPE_LEN	 2
#define MAC_ADDR_LEN	 6
#define BUFFER_LEN	 30

typedef unsigned char MacAddress[MAC_ADDR_LEN];
extern int errno;

int main()
{
	int sockFd = 0, retValue = 0;
	char buffer[BUFFER_LEN], dummyBuf[50];
	struct sockaddr_ll destAddr;
	short int etherTypeT = htons(0x8200);

	//MacAddress localMac = {0x38,0x60,0x77,0x13,0xef,0x9c};
	MacAddress localMac = {0x33,0x60,0x77,0x13,0xef,0x9c};
	MacAddress destMac = {0x32,0x60,0x77,0x13,0xef,0x9c};
	//MacAddress destMac = {0x14,0xda,0xe9,0x12,0x15,0x7d};

	memset(&destAddr, 0, sizeof(struct sockaddr_ll));
	memset(buffer, 0, BUFFER_LEN);

	if((sockFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		printf("ERROR! socket() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}
	printf("Socket creation success.\n");


	destAddr.sll_family = PF_PACKET;
	destAddr.sll_protocol = htons(ETH_P_ALL);
	destAddr.sll_halen = 6;
	destAddr.sll_ifindex = 2;
	memcpy(&(destAddr.sll_addr), destMac, MAC_ADDR_LEN);

#if 1
	/* Ethernet Header Construction */
	memcpy(buffer, localMac, MAC_ADDR_LEN);
	memcpy((buffer+MAC_ADDR_LEN), destMac, MAC_ADDR_LEN);
	memcpy((buffer+(2*MAC_ADDR_LEN)), &(etherTypeT), sizeof(etherTypeT));
#endif
	printf("2 Socket creation success.\n");

	/* Add some data */
	memset(dummyBuf, 0xa0, sizeof(dummyBuf));
	memcpy((buffer+ETHERTYPE_LEN+(2*MAC_ADDR_LEN)), dummyBuf, 50);
	printf("3 Socket creation success.\n");
	if((retValue = sendto(sockFd, buffer,64, 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
			printf("ERROR! sendto() call failed (Error No: %d \"%d\").\n", errno, error(errno));
			exit(1);
		}
		printf(" Send success (%d).\n", retValue);
	return(0);
}
