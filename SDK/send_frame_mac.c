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
	
	struct sockaddr_ll destAddr;
	short int etherTypeT = htons(0x8200);
	//short int etherTypeT = htons(0x9000);
	
	MacAddress localMac = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};
	//MacAddress destMac = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};
	//MacAddress destMac = {0x14, 0xda, 0xe9, 0x12, 0x15, 0x7d};
	MacAddress destMac = {0xe0, 0x69, 0x95, 0xf4, 0xd9, 0xb0};

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

#if 0	
	destAddr.sll_addr[0]  = 0x38;/*not used*/
	destAddr.sll_addr[1]  = 0x60;/*not used*/
	destAddr.sll_addr[2]  = 0x77;/*not used*/
	destAddr.sll_addr[3]  = 0x13;/*not used*/
	destAddr.sll_addr[4]  = 0xef;/*not used*/
	destAddr.sll_addr[5]  = 0x9c;/*not used*/

	/*MAC - end*/
	destAddr.sll_addr[6]  = 0x00;/*not used*/
	destAddr.sll_addr[7]  = 0x00;/*not used*/

	/*set the frame header*/
	memcpy((void*)buffer, (void*)dest_mac, ETH_ALEN);
	memcpy((void*)(buffer+ETH_ALEN), (void*)src_mac, ETH_ALEN);
	eh->h_proto = 0x00;
	/*fill the frame with some data*/
	for (j = 0; j < 1500; j++) {
			data[j] = (unsigned char)((int) (255.0*rand()/(RAND_MAX+1.0)));
	}

#endif 

	/* Ethernet Header Construction */
	memcpy(buffer, destMac, MAC_ADDR_LEN);
	memcpy((buffer+MAC_ADDR_LEN), localMac, MAC_ADDR_LEN);
	memcpy((buffer+(2*MAC_ADDR_LEN)), &(etherTypeT), sizeof(etherTypeT));

	/* Add some data */
	memset(dummyBuf, 'n', sizeof(dummyBuf));
	memcpy((buffer+ETHERTYPE_LEN+(2*MAC_ADDR_LEN)), dummyBuf, (BUFFER_LEN - 10));

	while(1){
		//sleep(5);
		if((retValue = sendto(sockFd, buffer, (BUFFER_LEN - 10), 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
		printf("ERROR! sendto() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}
	printf("Send success (%d).\n", retValue);
	}

	return(0);
}
