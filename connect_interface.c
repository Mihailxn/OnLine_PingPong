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
	int yMax, xMax;
	
    getmaxyx(stdscr, yMax, xMax);
	curs_set(FALSE);
	WINDOW *connect_load_wnd;
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	connect_load_wnd = newwin(5, xMax/2, yMax/4, xMax/4);
	wbkgd(connect_load_wnd, COLOR_PAIR(5));
	mvwprintw(connect_load_wnd, 2,1, "\tExpection of an opponent\t");
    wrefresh(connect_load_wnd);
    //тут установить соединение, 
    //отправив nikname_1 
    //и получив nikname_2
	
	//int sockfd, slen = sizeof(serv_addr);
	int portnum = 30022;
	
	bzero((char *) &CTS, sizeof(CTS));
	bzero((char *) &STC, sizeof(STC));
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd <= 0){
		perror ("socket");
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	strncpy(CTS.nick, nikname_1, sizeof(CTS.nick));
	
	sendto(sockfd, &CTS, sizeof(CTS), 0, (struct sockaddr *)&serv_addr, slen);
	/*mvwprintw(connect_load_wnd, 2,1, "\tInfoSent\t");
	wrefresh(connect_load_wnd);*/
	recvfrom(sockfd, &STC, sizeof(STC), 0, (struct sockaddr *)&serv_addr, &slen);
	nikname_2 = STC.nick;
	gCTS.number = STC.number;
	/*mvwprintw(connect_load_wnd, 2,1, "\tGot: %s\t", STC.nick);
	gCTS.number = STC.number;*/
	wrefresh(connect_load_wnd);
	
    getch();//временнно
	delwin(connect_load_wnd);
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
		default:
			break;
	}
}

void receiver(short *boll, short *score, short *y_play_1, short *y_play_2){
	int swaptmp;
	if (recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr *)&serv_addr, &slen) > 0){
			if (STC.number == 2){
				gSTC.x_ball = 60 - gSTC.x_ball;
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
			
		}
}
