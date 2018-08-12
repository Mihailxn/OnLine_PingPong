#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "pong.h"

struct sockaddr_in serv_addr;
int sockfd, slen = sizeof(serv_addr), goals;

void teke_connect(char *nikname_1, char *nikname_2){
	
	char str[] = "Expection of an opponent";
	int portnum = 30022;
	
	bzero((char *) &CTS, sizeof(CTS));
	bzero((char *) &STC, sizeof(STC));
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	loading(str);//отображение окна ожидания
 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd <= 0){
		perror ("socket");
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	strncpy(CTS.nick, nikname_1, sizeof(CTS.nick));
	CTS.here = 'H';
	sendto(sockfd, &CTS, sizeof(CTS), 0, (struct sockaddr *)&serv_addr, slen);

	/*
	if (getch()==10){
		CTS.here = 'E';
		sendto(sockfd, &CTS, sizeof(CTS), 0, (struct sockaddr *)&serv_addr, slen);
		erase();
		refresh();
		return();
	}
	*/
	
	recvfrom(sockfd, &STC, sizeof(STC), 0, (struct sockaddr *)&serv_addr, &slen);
	
	gCTS.number = STC.number;
	strncpy(nikname_2, STC.nick, sizeof(STC.nick));
	
	erase();
	refresh();	
}


void sender(char y_play){
	switch(y_play){
		case 'u':
			gCTS.move = 'U';
			sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
			break;
		case 'd':
			gCTS.move = 'D';
			sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
			break;
		case 'g':
			gCTS.move = 'G';
			sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
			break;
		default:
			break;
	}
}

void receiver(short *boll, short *score, short *y_play_1, short *y_play_2){
	int swaptmp;
	if (recvfrom(sockfd, &gSTC, sizeof(gSTC), MSG_DONTWAIT, (struct sockaddr *)&serv_addr, &slen) > 0){
		if (STC.number == 2){
			gSTC.x_ball = 76 - gSTC.x_ball;
			swaptmp = gSTC.y_player1;
			gSTC.y_player1 = gSTC.y_player2;
			gSTC.y_player2 = swaptmp;
		}
		boll[0] = gSTC.y_ball;
		boll[1] = gSTC.x_ball;
		*y_play_1 = gSTC.y_player2;
		*y_play_2 = gSTC.y_player1;
		if (gSTC.res == 'V')
			score[0]++;
		else if (gSTC.res == 'L')
			score[1]++;
		else if (gSTC.res == 'C')
			score[0] = 15;
		//ЗДЕСЬ ЗНАЧЕНИЯ ОЧЕНЬ СТАБИЛЬНЫЕ
	}
	/* 
	 * А здесь сама не стабильность
	 * Это значит, что корявые значения появляются только когда условие не выполняется
	 */
}
