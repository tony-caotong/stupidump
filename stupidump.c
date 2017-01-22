/**
 *	stupid dump: 
 *		Very very simple ethernet dump codes.
 *			by Cao Tong <tony.caotong@gmail.com>
 *			@ 2017-01-20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>

#include <arpa/inet.h>
#include <signal.h>

#define MAX_PKT_LEN (2<<16)
int Quit = 0;

void _sig_handle(int sig)
{
	Quit = 1;
}

int main(int argc, char** argv)
{
	int fd;
	struct sockaddr_ll addr;
	char buf[MAX_PKT_LEN];
	int len = 0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <ifname>\n", argv[0]);
		return -1;
	}

	fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd < 0) {
		perror("socket: ");
		return -1;
	}
	
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(ETH_P_ALL);

	struct ifreq req;
	strncpy(req.ifr_name, argv[1], sizeof(req.ifr_name)-1);

	if (ioctl(fd, SIOCGIFINDEX, &req) < 0) {
		perror("ioctl");
		goto err;
	}
	addr.sll_ifindex = req.ifr_ifindex;

	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("bind");
		goto err;
	}
	
	signal(SIGINT, _sig_handle);
	while (!Quit) {
		len = recv(fd, buf, MAX_PKT_LEN, 0);
		if (len < 0) {
			perror("recv: ");
			goto err;
		}
		fprintf(stderr, "recv - len[%d]\n", len);
	}
err:
	close(fd);
	fprintf(stderr, "exit.\n");
	return 0;
}
