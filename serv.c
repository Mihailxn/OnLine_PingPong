#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define X_FIELD 60//Размер поля x координаты
#define Y_FIELD 30//Размер поля y координаты
#define MID_RACKET 3//Размер середины ракетки (только нечётное число)
#define SIDE_RACKET 2//Размер боковой части ракетки
#define END_RACKET 1//Размер крайней части ракетки

 struct Vector//Структура движения шарика в следующий такт времени
{
    short x;
    short y;
};
 struct ClientToServerConnect
{
    char nikname[15];//Имя Игорька
}/*CTSC*/;
 struct ServerToClientAccept
{
    char nikname[15];//Имя противника
    short number;//Номер Игорька
}/*STCA*/;
 struct ClientToServerGame
{
    char act;
}/*CTSG*/;
 struct ServerToClientGame
{
    short int x_ball;
    short int y_ball;
    short int y_play_1;
    short int y_play_2;
    char status;
}/*STCG*/;


struct ListenerArguments
{
    short int *y_play;
    int listener_1;
    int client_address_size;
    struct sockaddr_in *client;
};

//Функция слушателя игроков, принимает от них сообщения
void *listener_fn(void *arguments)
{
    struct ListenerArguments *arg=(struct ListenerArguments *)arguments;
    struct ClientToServerGame CTSG;
    bzero((char* )&CTSG, sizeof(CTSG));
    ssize_t status;
    while(1)
    {
	//Без мьютексов так как изменяет данные только один данный поток, а чтением можно принебречь
	if(status = recvfrom(arg.listener, &CTSG, sizeof(CTSG)+1, 0, (struct sockaddr *) arg->client,&arg.client_address_size) <0)
	{
		printf("recvfrom()");
		exit(4);
	} 
	else 
	{
		if (CTSG.act = 'U')
		{
			arg->y_play += 3;
		} 
		if (CTSG.act = 'D')
		{
			arg->y_play -= 3;
		}
	}
    }
}

 int main()
{
	struct ClientToServerConnect CTSC;
	struct ServerToClientAccept STCA;
	int listener_1, listener_2, client_address_size_1,client_address_size_2;
	struct sockaddr_in addr, client_1,client_2;//endpoint сервера, первого клиента, второго клиента
	listener_1 = socket(AF_INET, SOCK_DGRAM, 0);
	if(listener_1 < 0)
	{
    	    perror("socket");
    	    exit(1);
	}
    
	addr.sin_family = AF_INET;
	addr.sin_port = htons(15302);
	inet_aton("127.0.0.1", &addr.sin_addr);
	if(bind(listener_1, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
    	    perror("bind");
    	    exit(2);
	}
    while(1)
    {
	client_address_size_1 = sizeof(client_1);
	if(recvfrom(listener_1, &CTSC, sizeof(CTSC)+1, 0, (struct sockaddr *) &client_1,&client_address_size_1) <0)
	{
    	    printf("recvfrom()");
    	    exit(4);
	}
	
    
	client_address_size_2 = sizeof(client_2);
	if(recvfrom(listener_1, &CTSC, sizeof(CTSC)+1, 0, (struct sockaddr *) &client_2,&client_address_size_2) <0)
	{
    	    printf("recvfrom()");
    	    exit(4);
	}
	pid_t pid;
	switch(pid=fork())
	{
	    case -1:
		perror("fork"); /* произошла ошибка */
		exit(1); /*выход из родительского процесса*/
	    case 0:
	    {
			STCA.number=1;
			strncpy(STCA.nikname,CTSC.nikname,sizeof(STCA.nikname));
		if (sendto(listener_1, &STCA, sizeof(STCA), 0,(struct sockaddr *)&client_1, sizeof(client_1)) < 0)
		{
		    printf("sendto()");
		    exit(2);
		}
		
		STCA.number=2;
		strncpy(STCA.nikname,CTSC.nikname,sizeof(STCA.nikname));
		if (sendto(listener_1, &STCA, sizeof(STCA), 0,(struct sockaddr *)&client_2, sizeof(client_2)) < 0)
		{
		    printf("sendto()");
		    exit(2);
		}
		//Начало игры_____________________________________________________________
		short x_ball=X_FIELD/2;
		short y_ball=Y_FIELD/2;
		struct Vector vct;
		short y_play_1=Y_FIELD/2;
		short y_play_2=Y_FIELD/2;
		struct ClientToServerGame CTSG;
		bzero((char* )&CTSG, sizeof(CTSG));
		struct ServerToClientGame STCG;
		bzero((char* )&STCG, sizeof(STCG));
		//ssize_t status;
		
		while(vct.x=rand()%3-1==0);
			vct.y=rand()%3-1;
			
			
		struct ListenerArguments LA1;//Аргумент для слушателя игрока слева
		LA1.listener=listener_1;
		LA1.client_address_size=client_address_size_1;
		LA1->client=&client_1;
		LA1->y_play=&y_play_1;
		
		struct ListenerArguments LA2;//Аргумент для слушателя игрока справа
		LA2.listener=listener_1;
		LA2.client_address_size=client_address_size_2;
		LA2->client=&client_2;
		LA2->y_play=&y_play_2;
		
		pthread_t p_listener_1;
		pthread_t p_listener_2;
		pthread_create(&p_listener_1,NULL,listener_fn,(void*)LA1);
		pthread_create(&p_listener_2,NULL,listener_fn,(void*)LA2);
		
		while(1)
		{
			STCG.status='G';
			STCG.x_ball=x_ball;
			STCG.y_ball=y_ball;
			STCG.y_play_1=y_play_1;
			STCG.y_play_2=y_play_2;
			
			/*if(status = recvfrom(listener_1, &CTSG, sizeof(CTSG)+1, MSG_DONTWAIT, (struct sockaddr *) &client_1,&client_address_size_1) <0)
			{
				printf("recvfrom()");
				exit(4);
			} else {
				if (CTSG.act = 'U'){
					y_play_1 += 3;
				} else {
					y_play_1 -= 3;
				}
			}
			if(status = recvfrom(listener_1, &CTSG, sizeof(CTSG)+1, MSG_DONTWAIT, (struct sockaddr *) &client_2,&client_address_size_2) <0)
			{
				printf("recvfrom()");
				exit(4);
			} else {
				if (CTSG.act = 'U'){
					y_play_2 += 3;
				} else {
					y_play_2 -= 3;
				}
			}*/
			
 		    //Отправляем сообщение игроку слева
		    if(x_ball>=X_FIELD)
				STCG.status='V';
		    if(x_ball<=0)
				STCG.status='L';
		    if (sendto(listener_1, &STCG, sizeof(STCG), 0,(struct sockaddr *)&client_1, sizeof(client_1)) < 0)
		    {
				printf("sendto()");
				exit(2);
		    }
		    //Отправляем сообщение игроку справа
		    if(x_ball>=X_FIELD)
		        STCG.status='L';
		    if(x_ball<=0)
				STCG.status='V';
		    if (sendto(listener_1, &STCG, sizeof(STCG), 0,(struct sockaddr *)&client_2, sizeof(client_2)) < 0)
		    {
				printf("sendto()");
				exit(2);
		    }
		    
		    //вынести в функцию но мне пока лень__________________________
		    //Если мяч улетел за правого игрока
		    if(x_ball>=X_FIELD)
		    {
			    x_ball=X_FIELD/2;
			    y_ball=Y_FIELD/2;
			    while(vct.x=rand()%3-1==0);
					vct.y=rand()%3-1;
		    }
		    //Если мяч улетел за левого игрока
		    if(x_ball<=0)
		    {
			    x_ball=X_FIELD/2;
			    y_ball=Y_FIELD/2;
			    while(vct.x=rand()%3-1==0);
					vct.y=rand()%3-1;
		    }
		    
		    //Мяч ударился о потолок
		    if(y_ball==1)
		    {
				vct.y=-vct.y;
		    }
		    //Мяч ударился о пол
		    if(y_ball==Y_FIELD-1)
		    {
				vct.y=-vct.y;
		    }
		    
		    //Мяч ударился о центр ракетки игрока справа
		    if(x_ball==X_FIELD-1&&(y_ball>=(y_play_2-MID_RACKET/2)&&y_ball<=(y_play_2+MID_RACKET/2)))
		    {
				vct.x=-1;
		    }
		    //Мяч ударился о верхний бок ракетки игрока справа
		    if(x_ball==X_FIELD-1&&(y_ball>(y_play_2+MID_RACKET/2)&&y_ball<=(y_play_2+MID_RACKET/2+SIDE_RACKET)))
		    {
				vct.x=-1;
				vct.y--;
		    }
		    //Мяч ударился о нижний бок ракетки игрока справа
		    if(x_ball==X_FIELD-1&&(y_ball<(y_play_2-MID_RACKET/2)&&y_ball>=(y_play_2-MID_RACKET/2-SIDE_RACKET)))
		    {
				vct.x=-1;
				vct.y++;
		    }
		    //Мяч ударился о верхний край ракетки игрока справа
		    if(x_ball==X_FIELD-1&&(y_ball>(y_play_2+MID_RACKET/2+SIDE_RACKET)&&y_ball<=(y_play_2+MID_RACKET/2+SIDE_RACKET+END_RACKET)))
		    {
				vct.x=-1;
				vct.y=vct.y-2;
		    }
		    //Мяч ударился о нижний край ракетки игрока справа
		    if(x_ball==X_FIELD-1&&(y_ball<(y_play_2-MID_RACKET/2-END_RACKET)&&y_ball>=(y_play_2-MID_RACKET/2-SIDE_RACKET-END_RACKET)))
		    {
				vct.x=-1;
				vct.y=vct.y+2;
		    }
		    
		    //Мяч ударился о центр ракетки игрока слева
		    if(x_ball==1&&(y_ball>=(y_play_1-MID_RACKET/2)&&y_ball<=(y_play_1+MID_RACKET/2)))
		    {
				vct.x=1;
		    }
		    //Мяч ударился о верхний бок ракетки игрока слева
		    if(x_ball==1&&(y_ball>(y_play_1+MID_RACKET/2)&&y_ball<=(y_play_1+MID_RACKET/2+SIDE_RACKET)))
		    {
				vct.x=-1;
				vct.y--;
		    }
		    //Мяч ударился о нижний бок ракетки игрока слева
		    if(x_ball==1&&(y_ball<(y_play_1-MID_RACKET/2)&&y_ball>=(y_play_1-MID_RACKET/2-SIDE_RACKET)))
		    {
				vct.x=-1;
				vct.y++;
		    }
		    //Мяч ударился о верхний край ракетки игрока слева
		    if(x_ball==1&&(y_ball>(y_play_1+MID_RACKET/2+SIDE_RACKET)&&y_ball<=(y_play_1+MID_RACKET/2+SIDE_RACKET+END_RACKET)))
		    {
				vct.x=-1;
				vct.y=vct.y-2;
		    }
		    //Мяч ударился о нижний край ракетки игрока слева
		    if(x_ball==1&&(y_ball<(y_play_1-MID_RACKET/2-END_RACKET)&&y_ball>=(y_play_1-MID_RACKET/2-SIDE_RACKET-END_RACKET)))
		    {
				vct.x=-1;
				vct.y=vct.y+2;
		    }
		    //Меняем позицию мячика
		    x_ball+=vct.x;
		    y_ball+=vct.y;
		    
		    sleep(5);//Пока подольше для отладки
		}
		exit(0);
	    }
	}
    }
}
