#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>

#define ETHERTYPE_LEN	 2
#define MAC_ADDR_LEN	 6
#define ETH_FRAME_LENGTH 	1518

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
	struct sockaddr_ll destAddr;
	int send_result = 0;
	int j = 0;	
	//short int etherTypeT = htons(0x8200);
	//short int etherTypeT = htons(0x9000);


	/*buffer for ethernet frame*/
	void* buffer = (void*)malloc(ETH_FRAME_LENGTH);	
	
	/*pointer to ethenet header*/
	unsigned char* etherhead = buffer;

	/*userdata in ethernet frame*/
	unsigned char* data = buffer + 14;
	
	/*another pointer to ethernet header*/
	struct ethhdr *eh = (struct ethhdr *)etherhead;
	
	/*our MAC address*/
	MacAddress src_mac = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};

	/*other host MAC address*/
	//MacAddress dest_mac =  {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};
	MacAddress dest_mac =  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


	/*prepare sockaddr_ll*/
	if((sockFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		printf("ERROR! socket() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}

	printf("Socket creation success.\n");
	
	/*prepare sockaddr_ll*/
	destAddr.sll_family = htons(AF_PACKET);
	/* suport all protocols */
	destAddr.sll_protocol = htons(ETH_P_ALL);
	/* mac address length */
	destAddr.sll_halen = 6;
	destAddr.sll_ifindex = 2;
	/*ARP hardware identifier is ethernet*/
	//destAddr.sll_hatype  = ARPHRD_ETHER;

	/*target is another host*/
	destAddr.sll_pkttype  = PACKET_OTHERHOST;

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
	
	/*set the frame header*/
	memcpy((void*)buffer, (void*)dest_mac, ETH_ALEN);

	memcpy((void*)(buffer+ETH_ALEN), (void*)src_mac, ETH_ALEN);
	eh->h_proto = 0x00;

	/*fill the frame with some data*/
	for (j = 0; j < 1500; j++) {
			data[j] = 1;
	}
	
	while(1){
		sleep(5);
		if((retValue = sendto(sockFd, buffer,ETH_FRAME_LEN, 0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0) {
		printf("ERROR! sendto() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}
	printf("Send success (%d).\n", retValue);
	}

	return(0);
}
