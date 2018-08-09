#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

struct ServerToClient{
	char nick[15];
	short number;
};

struct ClientToServer{
	char nick[15];
};

struct gameClientToServer{
	char move;
};

struct gameServerToClient{
	short x_ball;
	short y_ball;
	short y_player1;
	short y_player2;
	char res;
};

int main(){
	struct sockaddr_in serv_addr;
	struct ServerToClient STC;
	struct ClientToServer CTS;
	struct gameClientToServer gCTS;
	struct gameServerToClient gSTC;
	int sockfd = 0, slen = sizeof(serv_addr), portnum = 15228;
	
	bzero((char *) &CTS, sizeof(CTS));
	bzero((char *) &STC, sizeof(STC));
	bzero((char *) &gCTS, sizeof(gCTS));
	bzero((char *) &gSTC, sizeof(gSTC));
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd <= 0){
		perror ("socket");
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	printf ("Input nickname: ");
	fgets(CTS.nick, sizeof(CTS.nick), stdin);
	
	sendto(sockfd, &CTS, sizeof(CTS), 0, (struct sockaddr *)&serv_addr, slen);
	
	recvfrom(sockfd, &STC, sizeof(STC), 0, (struct sockaddr *)&serv_addr, &slen);
	printf("Got back:\n%s", STC.nick);
	printf("Number: %d\n", STC.number);

	return 0;
}
