#include <ncurses.h>
#include <stdlib.h>
#include "pong.h"

short menu(short highlight){
	initscr();
    start_color();
	curs_set(FALSE);//прячем курсор
	
	short enter = 0;
	char game_name[3][32]={{"Multiplayer Online Game"},{"Multiplayer Offline Game"},{"Game with Bot"}};
	short n_list=3;
	
	//задаём цветовые пары
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
	
	erase();
	wbkgd(stdscr, COLOR_PAIR(0));
    //для использования кнопочек
	keypad(stdscr,true);
	noecho();
	attron(COLOR_PAIR(1) | A_BOLD);
	mvprintw(3,2,
				"\tPPPPPP                            MM    MM EEEEEEE NN   NN UU   UU\n" 
				"\tPP   PP  oooo  nn nnn   gggggg    MMM  MMM EE      NNN  NN UU   UU\n" 
				"\tPPPPPP  oo  oo nnn  nn gg   gg    MM MM MM EEEEE   NN N NN UU   UU\n"
				"\tPP      oo  oo nn   nn ggggggg    MM    MM EE      NN  NNN UU   UU\n" 
				"\tPP       oooo  nn   nn      gg    MM    MM EEEEEEE NN   NN  UUUUU \n" 
				"\t                        ggggg                                     \n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    //рисуем рамку
	box(stdscr, 0, 0);
	//печать списка и перемещение по нему
	while(1){
		int i;
		for(i = 0; i < 3; i++){
			if(i == highlight){
				attron(A_REVERSE | COLOR_PAIR(1) | A_BOLD);
				mvprintw(i+13,5,"%s", game_name[i]);
				attroff(A_REVERSE | COLOR_PAIR(1) | A_BOLD);
			}else
				mvprintw(i+13,5,"%s", game_name[i]);
		}
		refresh();
		switch(getch()){
			case KEY_UP:
				highlight--;
				if (highlight == -1)
					highlight = 0;
				fbeep(600, 15);
				break;
			case KEY_DOWN:
				highlight++;
				if (highlight == n_list)
					highlight = n_list-1;
				fbeep(600, 15);
				break;
			case 10: // ENTER
				enter = 1;
				break;
			case 0x1B: // ESC (выход)
				highlight =-1;
				enter = 1;
				break;
			default:
				break;			
		}
		if(enter) break;
	}
    erase();
    refresh();
	endwin();
    return highlight;
}
