#include <ncurses.h>
#include <unistd.h>
#include <string.h> 
#include "pong.h"

#define RAZMER_Y 20//высота игровогшо поля
#define RAZMER_X 76//ширина игрового поля
#define MAX_NAME_LEN 15
#define LEFT 1
#define RIGHT 2

void online_play(short *boll, short *score, short *y_play_1, short *y_play_2, char y_play){
		/*
		 *Здесь нужно взять полученные от сервера значения:
		 * 	boll[0] - координата y мячика
		 * 	boll[1] - координата x мячика 
		 * 	y_play_1 - координата коретки соперника
		 * 	y_play_2 - координата коретки игрока
		 * 
		 * 	y_play может принимать одну из букв i,d,u
		 * 
		 * Для отражения координат(надо отразить X мячика) использовать можно:
		 * 	boll[0]=RAZMER_X-boll[0]; - должно работать
		 * 
		 * Значение score - нужно считать примерно так:
		 * 	if (boll[1] == RAZMER_X){
		 *	if (*y_play_1-boll[0]>1 || *y_play_1-boll[0]<-1){
		 *			score[0]++;
		 *		}
		 *	} else {
		 *	if(boll[1] == 0){
		 *		if (*y_play_2-boll[0]>1 || *y_play_2-boll[0]<-1){
		 *			score[1]++;
		 *		}
		 *	}	
		 * 
		 */
		error("Mode not yet implemented");//временно
		erase();//временно
		refresh();//временно
		endwin();//временно
}

int prediction(short *boll, short *v, int player, int position){
	int new_vector_x = v[1];
	int new_vector_y = v[0];
	int vector_x = v[1];
	int vector_y = v[0];
	int new_x = boll[1];
	int new_y = boll[0];
	int after_x = boll[1];
	int after_y = boll[0];
	if(position == LEFT){
		if((new_x < RAZMER_X/2) && (new_vector_x < 0)){
			while(after_x > 0){
				after_x += vector_x;
				if((after_y == RAZMER_Y) || (after_y == 0))
					vector_y *= -1;
				after_y += vector_y;
			}
			if(player > after_y)
				return -1;
			if(player < after_y)
				return +1;
			if(player == after_y)
				return 0;
		}
	}
	if(position == RIGHT){
		if((new_x > RAZMER_X/2) && (new_vector_x > 0)){
			while(after_x < (RAZMER_X+2)){
				after_x += vector_x;
				if((after_y == RAZMER_Y) || (after_y == 0))
					vector_y *= -1;
				after_y += vector_y;
			}
			if(player > after_y)
				return -1;
			if(player < after_y)
				return +1;
			if(player == after_y)
				return 0;
		}
	}
	return 0;
}

void offline_play(short *boll, short *v, short *score, short y_play_1, short y_play_2){
		//граница поля игры сверху или снизу
		if (boll[0] == RAZMER_Y || boll[0] == 0)
			v[0]*=-1;
		//граница справа
		if (boll[1] == RAZMER_X){
			if (y_play_1-boll[0]>1 || y_play_1-boll[0]<-1){
				score[0]++;
				boll[0] = RAZMER_Y/2;
				boll[1] = RAZMER_X/2; 
			}
			v[1]*=-1;
		} else
		//граница слева
		if(boll[1] == 0){
			if (y_play_2-boll[0]>1 || y_play_2-boll[0]<-1){
				score[1]++;
				boll[0] = RAZMER_Y/2;
				boll[1] = RAZMER_X/2;
			}
			v[1]*=-1; 
		}
		boll[0]+=v[0];
		boll[1]+=v[1];
}


void pong(short mod){
	//старт работы интерфейса
    initscr();
    start_color();
	curs_set(FALSE);//прячем курсор
	
	//окна
	WINDOW *score_wnd, *play_wnd, *box_wnd;
	int time = 0;
	short score[2] = {0};//счёт
	short y_play_1 = 10;//высоат первой коретки
	short y_play_2 = 10;//высота второй коретки
	short boll[2] = {RAZMER_Y/2,RAZMER_X/2};//координаты мяча
	short v[2] = {-1,1}; // направление вектора (вверх/вниз и влево/вправо)
	char y_play = 'i';//для онлайн режима, используется для указания действия игрока
	short esc = 0;
	
	//задаём цветовые пары
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_RED, COLOR_BLUE);
	
	//размеры экрана
	int yMax = RAZMER_Y+4;
    int xMax = RAZMER_X+4;
    //ставим окно игры на середину окна терминала
    int point_start_wnd[2]={0};
    getmaxyx(stdscr, point_start_wnd[1], point_start_wnd[0]);
    point_start_wnd[0]=(point_start_wnd[0]-xMax)/2;
    point_start_wnd[1]=(point_start_wnd[1]-yMax)/2;
    //создаём окна
	score_wnd = newwin(1, xMax, point_start_wnd[1], point_start_wnd[0]);
	box_wnd = newwin(yMax-1, xMax, point_start_wnd[1]+1, point_start_wnd[0]);
	play_wnd = newwin(yMax-3, xMax-2, point_start_wnd[1]+2, point_start_wnd[0]+1);
	wbkgd(score_wnd, COLOR_PAIR(1));
	wbkgd(box_wnd, COLOR_PAIR(2));
	wattron(score_wnd, A_BOLD);
	box(box_wnd, 0, 0);
	wrefresh(box_wnd);
	
	//void (*play)(short *, short *, short *, short *, short *);
	
    keypad(play_wnd, true);
	noecho();
	
	//если режим онлайн игры
	if (mod == 2){
		char nikname_1[MAX_NAME_LEN], nikname_2[MAX_NAME_LEN];
		enter_nikname(nikname_1);
		teke_connect(nikname_1, nikname_2);
		mvwprintw(score_wnd, 0, 0, nikname_1);
		mvwprintw(score_wnd, 0, xMax-strlen(nikname_2), nikname_2);
		wrefresh(score_wnd);
	}
	
	
	for (nodelay(play_wnd,1); esc==0; usleep(2000)){
    switch(mod){
			case 1:
			{
				if(++time%40==0){
					offline_play(boll, v, score, y_play_1, y_play_2);
				}
				switch(wgetch(play_wnd)){
					case KEY_UP:
						y_play_1--;
						if (y_play_1 == -1)
						y_play_1 = 0;
						break;
					case KEY_DOWN:
						y_play_1++;
						if (y_play_1 == yMax-3)
						y_play_1 = yMax-4;
						break;
					case 'w':
						y_play_2--;
						if (y_play_2 == -1)
						y_play_2 = 0;
						break;
					case 's':
						y_play_2++;
						if (y_play_2 == yMax-3)
						y_play_2 = yMax-4;
						break;
					case 'p':
						getchar();
						break;
					case 0x1B: // ESC (выход)
						esc =-1;
						break;
					default:
						break;			
				}
				break;
			}
			case 2:
			{	
				online_play(boll, score, &y_play_1, &y_play_2, y_play);
				switch(wgetch(play_wnd)){
					case KEY_UP:
						y_play = 'd';
						break;
					case KEY_DOWN:
						y_play = 'u';
						break;
					case 0x1B: // ESC (выход)
						esc =-1;
						break;
					default:
						break;			
				}
				return;//временно
			}
			case 3:
			{
				switch(wgetch(play_wnd)){
					case KEY_UP:
						y_play_1--;
						if (y_play_1 == -1)
						y_play_1 = 0;
						break;
					case KEY_DOWN:
						y_play_1++;
						if (y_play_1 == yMax-3)
						y_play_1 = yMax-4;
						break;
					case 'p': //пауза
						getchar();
						break;
					case 0x1B: // ESC (выход)
						esc =-1;
						break;
					default:
						break;
				}
				y_play_2 += prediction(boll, v, y_play_2, LEFT);
				//y_play_1 += prediction(boll, v, y_play_1, RIGHT);
			}
		}
		if (esc<0) break;
		werase(play_wnd);
		mvwprintw(score_wnd, 0, xMax/2-3, "%i | %i", score[0], score[1]);
		mvwvline(play_wnd,0,RAZMER_X/2,ACS_VLINE,RAZMER_Y+1);
		wattron(play_wnd,COLOR_PAIR(1) | A_BOLD);
		mvwprintw(play_wnd,boll[0],boll[1],"o");
		int i;
		for(i=-1;i<2;i++){
			mvwprintw(play_wnd,y_play_2+i,0,"|");
			mvwprintw(play_wnd,y_play_1+i,xMax-3,"|");
		}
		wattroff(play_wnd,COLOR_PAIR(1) | A_BOLD);
		wrefresh(score_wnd);
		wrefresh(play_wnd);
	}
	
    //завершение работы интерфейса
    delwin(score_wnd);
    delwin(play_wnd);
    delwin(box_wnd);
    erase();
    refresh();
    endwin();
}
