#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define RAZMER_Y 20//высота игровогшо поля
#define RAZMER_X 76//ширина игрового поля

#define WIN_RATE 70

#define WAIT 0
#define SCAN 1
#define RANDON 2
#define MOVEING 3

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
	int sockfd = 0, slen = sizeof(serv_addr), portnum = 30022, goals = 0;
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
        serv_addr.sin_port = htons(atoi(argv[2]));
        inet_aton(argv[1], &serv_addr.sin_addr);
    }else{
    	serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(portnum);
	    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    }

	strncpy(CTS.nick, "bot", sizeof("bot"));

	sendto(sockfd, &CTS, sizeof(CTS), 0, (struct sockaddr *)&serv_addr, slen);
	
	recvfrom(sockfd, &STC, sizeof(STC), 0, (struct sockaddr *)&serv_addr, &slen);
	
    srand(time(NULL));

    short before_x, before_y;
    short new_x = 0, new_y = 0;
    short after_x = 0, after_y = 0;
    short vector_x, vector_y;
    short status = WAIT, bot_y = 10, time = 0;
    while(1){
        if(status == WAIT){
            printf("%d\n",status);
            recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr*)&serv_addr, &slen);
            before_x = gSTC.x_ball;
            before_y = gSTC.y_ball;

            do{
                recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr*)&serv_addr, &slen);
                new_x = gSTC.x_ball;
                new_y = gSTC.y_ball;
            }while(before_x == new_x);

            if(before_x != new_x)
                status = SCAN;
        }
        
        if(status == SCAN){
            printf("%d\n",status);
            if(before_x > new_x)
                vector_x = -1;
            else vector_x = +1;

            if(before_y > new_y)
                vector_y = -1;
            if(before_y < new_y)
                vector_y = +1;
            if(before_y == new_y)
                vector_y == 0;

            if((vector_x > 0) && (new_x > RAZMER_X/2)){
                after_x = new_x;
                after_y = new_y;
                while(after_x < RAZMER_X-1){//настройка под логику сервера
                    after_x += vector_x;
                    after_y += vector_y;
                    if((after_y >= RAZMER_Y-1) || (after_y <= 1))//настройка под логику сервера
                        vector_y *= -1;
                }
                status = RANDON;
            }else status = WAIT;
        }

        if(status == RANDON){
            if(rand()%100 > WIN_RATE){
                after_y = rand()%(RAZMER_Y+1);
            }
            status = MOVEING;
        }

        if(status == MOVEING){
            recvfrom(sockfd, &gSTC, sizeof(gSTC), 0, (struct sockaddr*)&serv_addr, &slen);
            bot_y = gSTC.y_player2;
            new_x = gSTC.x_ball;
            printf("%d\n",status);
            printf("%d - %d\n", after_y, bot_y);
            
            if((after_y == bot_y) && (new_x < RAZMER_X/2))
                status = WAIT; 
            
            if(!(++time%40)){//таймер для замедление движения ракетки
                if(after_y > bot_y){
                    gCTS.move = 'D';
                    sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
                }

                if(after_y < bot_y){
                    gCTS.move = 'U';
                    sendto(sockfd, &gCTS, sizeof(gCTS), 0, (struct sockaddr *)&serv_addr, slen);
                }   
            }
        }
    }
	return 0;
}
