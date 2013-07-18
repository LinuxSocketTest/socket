#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netpacket/packet.h>
//#include <net/ethernet.h>
//#include <netinet/ether.h>

#define ETHERTYPE_LEN    2
#define MAC_ADDR_LEN     6
#define BUFFER_LEN       50
#define ETH_FRAME_LEN 	1518

int main()
{
	int s = 0, retValue = 0;  
	int j;	
	/*target address*/
	struct sockaddr_ll socket_address;
	unsigned char* etherhead;
	short int etherTypeT = htons(0x8200);
	int send_result = 0;
	/*buffer for ethernet frame*/
	void* buffer = (void*)malloc(ETH_FRAME_LEN);

	/*pointer to ethenet header*/
	etherhead = buffer;
	
	/*userdata in ethernet frame*/
	unsigned char* data = buffer + 14;	 

	/*another pointer to ethernet header*/
	struct ethhdr *eh = (struct ethhdr *)etherhead;	
	
	/*our MAC address*/
	unsigned char src_mac[6] = {0x38, 0x60, 0x77, 0x13, 0xef, 0x9c};
	
	/*other host MAC address*/
	unsigned char dest_mac[6] = {0xe0, 0x69, 0x95, 0xf4, 0xd9, 0xb0};
	
	if((s= socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		printf("ERROR! socket() call failed (Error No: %d \"%d\").\n", errno, error(errno));
		exit(1);
	}

	/*prepare sockaddr_ll*/
	/*RAW communication*/
	socket_address.sll_family   = PF_PACKET;	
	/*we don't use a protocoll above ethernet layer
	 *   ->just use anything here*/
	socket_address.sll_protocol = htons(ETH_P_ALL); 	
	/*index of the network device
	 * see full code later how to retrieve it*/
	socket_address.sll_ifindex  = 2;
	/*ARP hardware identifier is ethernet*/
	socket_address.sll_hatype   = ARPHRD_ETHER;
	/*target is another host*/
	socket_address.sll_pkttype  = PACKET_OTHERHOST;
	/*address length*/
	socket_address.sll_halen    = ETH_ALEN;
	
	/*MAC - begin*/
	socket_address.sll_addr[0]  = 0xe0;		
	socket_address.sll_addr[1]  = 0x69;		
	socket_address.sll_addr[2]  = 0x95;
	socket_address.sll_addr[3]  = 0xf4;
	socket_address.sll_addr[4]  = 0xd9;
	socket_address.sll_addr[5]  = 0xb0;

	/*MAC - end*/
	socket_address.sll_addr[6]  = 0x00;/*not used*/
	socket_address.sll_addr[7]  = 0x00;/*not used*/

	/*set the frame header*/
	memcpy((void*)buffer, (void*)dest_mac, ETH_ALEN);
	memcpy((void*)(buffer+ETH_ALEN), (void*)src_mac, ETH_ALEN);
	eh->h_proto = 0x00;
	
	/*fill the frame with some data*/
	for (j = 0; j < 1500; j++) {
			data[j] = j+1;
	}

	memset(buffer, 0xa0, sizeof(ETH_FRAME_LEN)); 	

	while(1){
	/*send the packet*/
	send_result = sendto(s, buffer, ETH_FRAME_LEN, 0, 
				      (struct sockaddr*)&socket_address, sizeof(socket_address));
	}
}
