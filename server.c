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
#include <unistd.h> 

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
    short number;
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
    //short int *y_play;
    int move_1;
    int move_2;
    int *listener_1;
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
	if(status = recv(*arg->listener_1, &CTSG, sizeof(CTSG), 0) <0)

	{
		printf("recvfrom()");
		exit(4);
	} 
	else 
	{
		if (CTSG.act == 'U')
		{
		    if(CTSG.number==1)
			arg->move_1 += 3;
		    else
			arg->move_2 += 3;
			printf("Got U\n");
		} 
		if (CTSG.act == 'D')
		{
		    if(CTSG.number==1)
			arg->move_1 -= 3;
		    else
			arg->move_2 -= 3;
			printf ("GOt D\n");
		}
	}
    }
}
    
 int main()
{	int port=30003;
	struct ClientToServerConnect CTSC;
	struct ServerToClientAccept STCA;
	int listener_2, client_address_size_1,client_address_size_2;
	struct sockaddr_in addres, client_1,client_2;//endpoint сервера, первого клиента, второго клиента
	listener_2 = socket(AF_INET, SOCK_DGRAM, 0);
	if(listener_2 < 0)
	{
    	    perror("socket");
    	    exit(1);
	}
    
	addres.sin_family = AF_INET;
	addres.sin_port = htons(port);
	inet_aton("127.0.0.1", &addres.sin_addr);
	if(bind(listener_2, (struct sockaddr *)&addres, sizeof(addres)) < 0)
	{
    	    perror("bind");
    	    exit(2);
	}
	
<<<<<<< HEAD
	
    while(1)
    {
	client_address_size_2 = sizeof(client_1);
	if(recvfrom(listener_2, &CTSC, sizeof(CTSC)+1, 0, (struct sockaddr *) &client_1,&client_address_size_1) <0)
=======
    //while(1)
    //{
	client_address_size_1 = sizeof(client_1);
	if(recvfrom(listener_1, &CTSC, sizeof(CTSC)+1, 0, (struct sockaddr *) &client_1,&client_address_size_1) <0)
>>>>>>> d5c5ecb33b16441cf4b6683b631485c92f4e03f2
	{
    	    printf("recvfrom()");
    	    exit(4);
	}
	strncpy(STCA.nikname,CTSC.nikname,sizeof(STCA.nikname));
    
	client_address_size_2 = sizeof(client_2);
	if(recvfrom(listener_2, &CTSC, sizeof(CTSC)+1, 0, (struct sockaddr *) &client_2,&client_address_size_2) <0)
	{
    	    printf("recvfrom()");
    	    exit(4);
	}
	port++;
	pid_t pid;
	switch(pid=fork())
	{
	    case -1:
		perror("fork"); /* произошла ошибка */
		exit(1); /*выход из родительского процесса*/
	    case 0:
	    {
		int listener_1;
		listener_1 = socket(AF_INET, SOCK_DGRAM, 0);
		if(listener_1 < 0)
		{
    		    perror("socket");
    		    exit(1);
		}	
		struct sockaddr_in addr;
		//Биндим новый сокет, чтобы на старый ничего не шло и было возможно подключение новых клиентов.
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_aton("127.0.0.1", &addr.sin_addr);
		if(bind(listener_1, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
    		    perror("bind");
    		    exit(2);
		}
		
		
		STCA.number=2;
		if (sendto(listener_1, &STCA, sizeof(STCA), 0,(struct sockaddr *)&client_2, sizeof(client_2)) < 0)
		{
		    printf("sendto()");
		    exit(2);
		}
		
		STCA.number=1;
		strncpy(STCA.nikname,CTSC.nikname,sizeof(STCA.nikname));
		if (sendto(listener_1, &STCA, sizeof(STCA), 0,(struct sockaddr *)&client_1, sizeof(client_1)) < 0)
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
		
		while((vct.x=rand()%3-1)==0);
			vct.y=rand()%3-1;
			
			
		struct ListenerArguments LA1;//Аргумент для слушателя игрока слева
		bzero((char*)&LA1, sizeof(LA1));
		LA1.listener_1=&listener_1;
		LA1.client_address_size=client_address_size_1;
		LA1.client=&client_1;
		LA1.move_1=y_play_1;
		LA1.move_2=y_play_1;
		//LA1.y_play=&y_play_1;
		
		
		//struct ListenerArguments LA2;//Аргумент для слушателя игрока справа
		//bzero((char*)&LA2, sizeof(LA2));
		//LA2.listener_1=&listener_1;
		//LA2.client_address_size=client_address_size_2;
		//LA2.client=&client_2;
		//LA2.move=y_play_2;
		//LA2.y_play=&y_play_2;
		
		pthread_t p_listener_1;
		//pthread_t p_listener_2;
		pthread_create(&p_listener_1,NULL,listener_fn,(void*)&LA1);
		//pthread_create(&p_listener_2,NULL,listener_fn,(void*)&LA2);
		
		while(1)
		{
		    int i;
		    for(i=0;i<40;i++)
		    {
			/*if (LA1.move == 1){
				y_play_1 += 3;
			}
			if (LA1.move == 2){
				y_play_1 -= 3;
			}
			if (LA2.move == 1){
				y_play_2 += 3;
			}
			if (LA2.move == 2){
				y_play_2 -= 3;
			}*/
			y_play_1=LA1.move_1;
			y_play_2=LA1.move_2;
			/*
			printf("LA2 = %d\n", LA2.move);
			printf("LA1 = %d\n", LA1.move);
			*/
			STCG.status='G';
			STCG.x_ball=x_ball;
			STCG.y_ball=y_ball;
			STCG.y_play_1=y_play_1;
			STCG.y_play_2=y_play_2;
			
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
			usleep(12000);
		    }
		    //вынести в функцию но мне пока лень________________
		    //Если мяч улетел за правого игрока
		    if(x_ball>=X_FIELD)
		    {
			    x_ball=X_FIELD/2;
			    y_ball=Y_FIELD/2;
			    //while(vct.x=rand()%3-1==0);
			    vct.x=-1;
					vct.y=rand()%3-1;
		    }
		    //Если мяч улетел за левого игрока
		    if(x_ball<=0)
		    {
			    x_ball=X_FIELD/2;
			    y_ball=Y_FIELD/2;
			    //while(vct.x=rand()%3-1==0);
			    vct.x=1;
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
		    
		    usleep(12000);//Пока подольше для отладки
		}
		int status[2];
		pthread_join(p_listener_1,(void **)&status[1]);
		//pthread_join(p_listener_2,(void **)&status[2]);
		exit(0);
	    }
	}
    }
}
