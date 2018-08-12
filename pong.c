#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h> 
#include "pong.h"

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
				y_play = 'g';
				switch(wgetch(play_wnd)){
					case KEY_UP:
						y_play = 'u';
						break;
					case KEY_DOWN:
						y_play = 'd';
						break;
					case 0x1B: // ESC (выход)
						esc =-1;
						break;
					default:
						break;			
				}
				sender(y_play);
				receiver(boll, score, &y_play_1, &y_play_2);
			}
			case 3:
			{
				if(++time%40==0){
					offline_play(boll, v, score, y_play_1, y_play_2);
				}
				switch(wgetch(play_wnd)){
					case KEY_UP:
						y_play_2--;
						if (y_play_2 == -1)
						y_play_2 = 0;
						break;
					case KEY_DOWN:
						y_play_2++;
						if (y_play_2 == yMax-3)
						y_play_2 = yMax-4;
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
				y_play_1 += bot(boll, v, RIGHT, y_play_1);
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
