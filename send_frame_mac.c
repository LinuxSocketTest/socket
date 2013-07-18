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

#define ETHERTYPE_LEN	 2
#define MAC_ADDR_LEN	 6
#define BUFFER_LEN	 	1500	

typedef unsigned char MacAddress[MAC_ADDR_LEN];
extern int errno;
/* 
 *
 *   For client: src is own address, dest is address to send packets to.
 *     For server: src is address of sender, dest is own address.        
 *  */


int main()
{
	int sockFd = 0, retValue = 0;

	/*buffer for ethernet frame*/
	char* buffer = (void*)malloc(BUFFER_LEN);
	char* dummyBuf = (void*)malloc(BUFFER_LEN);
	int i = 0;	
	struct sockaddr_ll destAddr;
	short int etherTypeT = htons(0x8200);
	
	MacAddress localMac = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};
	MacAddress destMac = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};

	memset(&destAddr, 0, sizeof(struct sockaddr_ll));
	memset(buffer, 0, BUFFER_LEN);

	if((sockFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		printf("ERROR! socket() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}
	printf("Socket creation success.\n");

	destAddr.sll_family = htons(AF_PACKET);
	destAddr.sll_protocol = htons(ETH_P_ALL);
	destAddr.sll_halen = 6;
	destAddr.sll_ifindex = 2;
	destAddr.sll_hatype  = ARPHRD_ETHER;

	memcpy(&(destAddr.sll_addr), localMac, MAC_ADDR_LEN);


	/* Ethernet Header Construction */
	memcpy(buffer, destMac, MAC_ADDR_LEN);
	memcpy((buffer+MAC_ADDR_LEN), localMac, MAC_ADDR_LEN);
	memcpy((buffer+(2*MAC_ADDR_LEN)), &(etherTypeT), sizeof(etherTypeT));

	/* Add some data */
	memset(dummyBuf, 'n', sizeof(dummyBuf));
	memcpy((buffer+ETHERTYPE_LEN+(2*MAC_ADDR_LEN)), dummyBuf, (BUFFER_LEN));

	while(1){
		if((retValue = sendto(sockFd, buffer,1400, 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
		printf("ERROR! sendto() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}
	printf("Send success (%d).\n", retValue);
	}

	return(0);
}
