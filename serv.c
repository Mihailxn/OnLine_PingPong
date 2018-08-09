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
#define X_FIELD 30//Размер поля x координаты
#define Y_FIELD 60//Размер поля y координаты


struct ClientToServerConnect
{
    char nikname[15];//Имя Игорька
}/*CTSC*/;

struct ServerToClientAccept
{
    char nikname[15];//Имя противника
    short number;//Номер Игорька
}/*STCA*/;

typedef struct ClientToServerGame
{
    char act;
}CTSG;

typedef struct ServerToClientGame
{
    short int x_boll;
    short int y_boll;
    short int x_play_1;
    short int x_play_2;
    char status;
}STCG;

int main()
{
    while(1)
    {
	struct ClientToServerConnect CTSC;
	struct ServerToClientAccept STCA;
	int listener_1, listener_2, client_address_size_1,client_address_size_2;
	struct sockaddr_in addr, client_1,client_2;//endpoint сервера, первого клиента, второго клиента
	listener_1 = socket(AF_INET, SOCK_DGRAM, 0);
	//listener_2 = socket(AF_INET, SOCK_DGRAM, 0);
	if(listener_1 < 0)
	{
    	    perror("socket");
    	    exit(1);
	}
    //if(listener_2 < 0)
    //{
    //    perror("socket");
    //    exit(1);
    //}
    
	addr.sin_family = AF_INET;
	addr.sin_port = htons(15226);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//Временный локальный адрес для удобной отладки
    
	if(bind(listener_1, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
    	    perror("bind");
    	    exit(2);
	}
    //if(bind(listener_2, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    //{
    //    perror("bind");
    //    exit(2);
    //}
    
	client_address_size_1 = sizeof(client_1);
	if(recvfrom(listener_1, &CTSC, sizeof(&CTSC), 0, (struct sockaddr *) &client_1,&client_address_size_1) <0)
	{
    	    printf("recvfrom()");
    	    exit(4);
	}
	STCA.number=1;
	strncpy(STCA.nikname,CTSC.nikname,sizeof(STCA.nikname));
    
	client_address_size_2 = sizeof(client_2);
	if(recvfrom(listener_1, &CTSC, sizeof(&CTSC), 0, (struct sockaddr *) &client_2,&client_address_size_2) <0)
	{
    	    printf("recvfrom()");
    	    exit(4);
	}
	pid_t pid;
	//int rv;
	switch(pid=fork())
	{
	    case -1:
		perror("fork"); /* произошла ошибка */
		exit(1); /*выход из родительского процесса*/
	    case 0:
	    {
		
		if (sendto(listener_1, &STCA, sizeof(&STCA), 0,(struct sockaddr *)&client_1, sizeof(client_1)) < 0)
		{
		    printf("sendto()");
		    exit(2);
		}
		
		STCA.number=2;
		strncpy(STCA.nikname,CTSC.nikname,sizeof(STCA.nikname));
		if (sendto(listener_1, &STCA, sizeof(&STCA), 0,(struct sockaddr *)&client_2, sizeof(client_2)) < 0)
		{
		    printf("sendto()");
		    exit(2);
		}
		                                                        
		exit(0);
	    }
	    //default:
	}
    }
    
    
    
    
    
    
    
    
    
    
}