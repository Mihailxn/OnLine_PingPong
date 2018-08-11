#include <ncurses.h>
#include <unistd.h>
#include "pong.h"

#define RAZMER_Y 20//высота игровогшо поля
#define RAZMER_X 74//ширина игрового поля

void offline_play(short *boll, short *v, short *score, short y_play_1, short y_play_2){
		//граница поля игры сверху или снизу
		if (boll[0] == RAZMER_Y || boll[0] == 0)
			v[0]*=-1;
		//граница справа
		if (boll[1] == RAZMER_X+1){
			if (y_play_1-boll[0]>1 || y_play_1-boll[0]<-1){
				score[1]++;
				boll[0] = RAZMER_Y/2;
				boll[1] = RAZMER_X/2; 
			}
			v[1]*=-1;
		} else
		//граница слева
		if(boll[1] == 0){
			if (y_play_2-boll[0]>1 || y_play_2-boll[0]<-1){
				score[0]++;
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
	
	if (mod!=1)
	{
		error("Mode not yet implemented");
		erase();
		refresh();
		endwin();
		return;
	}
	
	//окна
	WINDOW *score_wnd, *play_wnd, *box_wnd;
	int time = 0;
	short score[2] = {0};//счёт
	short y_play_1 = 10;//высоат первой коретки
	short y_play_2 = 10;//высота второй коретки
	short boll[2] = {RAZMER_Y/2,RAZMER_X/2};//координаты мяча
	short v[2] = {-1,1}; // направление вектора (вверх/вниз и влево/вправо)
	short esc = 0;
	
	//задаём цветовые пары
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_RED, COLOR_BLUE);
	
	//размеры экрана
	int yMax = RAZMER_Y+4;
    int xMax = RAZMER_X+6;
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
	
    keypad(play_wnd, true);
	noecho();
	
	for (nodelay(play_wnd,1); esc==0; usleep(2000)){
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
		if (esc<0) break;
		werase(play_wnd);
		mvwprintw(score_wnd, 0, xMax/2-3, "%i | %i", score[0], score[1]);
		mvwvline(play_wnd,0,RAZMER_X/2+1,ACS_VLINE,RAZMER_Y+1);
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
