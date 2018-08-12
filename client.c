#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

struct ServerToClient{
	char nick[15];
	short number;
};

struct ClientToServer{
	char nick[15];
};

struct gameClientToServer{
	char  move;
	short number;
};

struct gameServerToClient{
	short x_ball;
	short y_ball;
	short y_player1;
	short y_player2;
	char res;
};

int exitflag = 0;

int main(){
	struct sockaddr_in serv_addr;
	struct ServerToClient STC;
	struct ClientToServer CTS;
	struct gameClientToServer gCTS;
	struct gameServerToClient gSTC;
	int sockfd = 0, slen = sizeof(serv_addr), portnum = 30022, goals = 0;
	short swaptmp;
	char key;
	pid_t pid;
	
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
	printf("Opp nick:%s\nNumber: %d\n", STC.nick, STC.number);
	gCTS.number = STC.number;
	
	pid = fork();
	switch(pid){
		case -1:
			perror("fork");
			exit(0);
		case 0:
			while (!exitflag){
				key = getchar();
				getchar();
				switch(key){
					case 'w':
						gCTS.move = 'U';
						puts("sending U");
						sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
						break;
					case 's':
						gCTS.move = 'D';
						puts("sending D");
						sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
						break;
					default:
						puts("wrong button");
				}
			}
			break;
		default:
			while (goals < 5){
				if (recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr *)&serv_addr, &slen) > 0){
					if (STC.number == 2){
						gSTC.x_ball = 60 - gSTC.x_ball;
						swaptmp = gSTC.y_player1;
						gSTC.y_player1 = gSTC.y_player2;
						gSTC.y_player2 = swaptmp;
					}
					puts("got back struct");
					printf ("x_ball = %d\ny_ball = %d\nypl1 = %d\nypl2 = %d\n", gSTC.x_ball, gSTC.y_ball,
						gSTC.y_player1,gSTC.y_player2);
				}
				//redraw screen();
			}
			exitflag = 1;
			break;
	}
	
	return 0;
}
