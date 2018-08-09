#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

struct structback{
	char nick[15];
	short number;
};

struct structsend{
	char nick[15];
};

int main(){
	struct sockaddr_in serv_addr;
	struct structback cnctd;
	struct structsend firstcon;
	
	memset (cnctd.nick, 0, sizeof(cnctd.nick));
	cnctd.number = 0;
	
	int sockfd, i, slen = sizeof(serv_addr), portnum = 8888;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	printf ("Input nickname: ");
	bzero((char *) &firstcon, sizeof(firstcon));
	fgets(firstcon.nick, sizeof(firstcon.nick), stdin);
	
	sendto(sockfd, &firstcon, sizeof(firstcon), 0, (struct sockaddr *)&serv_addr, slen);
	
	recvfrom(sockfd, &cnctd, sizeof(cnctd), 0, (struct sockaddr *)&serv_addr, &slen);
	printf("Got back: %s", cnctd.nick);
	printf("Number: %d\n", cnctd.number);

	return 0;
}
