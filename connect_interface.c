#include <ncurses.h>
#include "pong.h"

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
    getch();//временнно
	nikname_2 = "test_nik";//временно
	delwin(connect_load_wnd);
}
