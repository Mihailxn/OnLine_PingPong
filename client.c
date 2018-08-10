#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
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
	int move;
};

struct gameServerToClient{
	short x_ball;
	short y_ball;
	short y_player1;
	short y_player2;
	char res;
};

int exitflag = 0;

int main2(){
	struct sockaddr_in serv_addr;
	struct ServerToClient STC;
	struct ClientToServer CTS;
	struct gameClientToServer gCTS;
	struct gameServerToClient gSTC;
	int sockfd = 0, slen = sizeof(serv_addr), portnum = 15302, goals = 0;
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
	printf("My nick:%s\nNumber: %d\n", STC.nick, STC.number);
	
	pid = fork();
	if (pid == -1){
		perror("fork");
		exit(0);
	} else {
		if (pid == 0){
			char key;
			while (!exitflag){
				key = getchar();
				if (key == 'w'){
						gCTS.move = 1;
						puts("sending 1");
						sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
				} else {
					if (key == 's'){
						gCTS.move = 2;
						puts("sending 2");
						sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
					} else {
						puts("vse ploha");
					}
				}
			}
		} else {
			while (goals < 5){
				sleep(3);
				if (recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr *)&serv_addr, &slen) > 0);
				puts("got back struct");
				printf ("x_ball = %d\ny_ball = %d\nxpl1 = %d\nypl1 = %d\n", gSTC.x_ball, gSTC.y_ball,
						gSTC.y_player1,gSTC.y_player2);
				//redraw screen();
			}
			exitflag = 1;
		}
	}
	return 0;
}
