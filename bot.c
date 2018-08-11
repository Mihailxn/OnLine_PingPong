#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RAZMER_Y 20//высота игровогшо поля
#define RAZMER_X 74//ширина игрового поля

#define MAX_STEP 18//шаги для закрытия поля

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

int main(int argc, char const *argv[]){
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
    
    if(argc == 3){
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(atoi(argv[3]));
        inet_aton(argv[2], &serv_addr.sin_addr);
    }else{
    	serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(portnum);
	    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    }

	strncpy(CTS.nick, "bot", sizeof("bot"));

	sendto(sockfd, &CTS, sizeof(CTS), 0, (struct sockaddr *)&serv_addr, slen);
	
	recvfrom(sockfd, &STC, sizeof(STC), 0, (struct sockaddr *)&serv_addr, &slen);
	
    int before_x, before_y, new_x = 0, new_y = 0 , after_x = 0, after_y = 0, bot_y;
    while(1){
        gCTS.move = 0;
        sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
        
        recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr*)&serv_addr, &slen);
        before_x = new_x;
        before_y = new_y;
        new_x = gSTC.x_ball;
        new_y = gSTC.y_ball;
        bot_y = gSTC.y_player2;

        //шарик приблежается к боту, шарик на его стороне поля, блокировка на повтор 
        if((before_x < new_x) && (new_x > RAZMER_X / 2) && (bot_y != after_y)){
            //напровление шарика по оси "y"
            int vector_y;
            if(before_y < new_y)
                vector_y = +1;
            else vector_y = -1;

            //ищем точку контакта шарика с ракеткой
            after_x = new_x;
            after_y = new_y;
            while(after_x < RAZMER_X){
                after_x += 1;
                after_y +=vector_y;
                if((after_y == RAZMER_Y) || (after_y == 0))
                    vector_y *= -1;
            }
            //двигаем ракетку
            while(after_y != bot_y){
                if(after_y < bot_y){
                    gCTS.move = 2;
                    sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
                    //придумать что то с задержками (чтоб ракетка не летала, а плавно передвигалась) тут или на сервере.
                }else{
                    gCTS.move = 1;
                    sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
                }
                recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr*)&serv_addr, &slen);
                bot_y = gSTC.y_player2;
                usleep(300);   
            }
        }
    }
	return 0;
}
