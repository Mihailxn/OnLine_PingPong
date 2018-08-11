#include <sys/ioctl.h>
#include <ncurses.h>
#include <unistd.h> 
#include "pong.h"

/*
void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}*/

void error(char *str){
	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
	curs_set(FALSE);
	WINDOW *error_wnd;
	init_pair(1, COLOR_BLACK, COLOR_RED);
	error_wnd = newwin(4, xMax/2, yMax/4, xMax/4);
	wbkgd(error_wnd, COLOR_PAIR(1));
	mvwprintw(error_wnd, 1,1, "%s", str);
    wrefresh(error_wnd);
    getch();
    //werase(error_wnd);
    delwin(error_wnd);
}

void print_load_game(){
	printf("\n\t  ██████╗ ███╗   ██╗      ██╗     ██╗███╗   ██╗███████╗\n"
			 "\t ██╔═══██╗████╗  ██║      ██║     ██║████╗  ██║██╔════╝\n"
			 "\t ██║   ██║██╔██╗ ██║█████╗██║     ██║██╔██╗ ██║█████╗  \n"
			 "\t ██║   ██║██║╚██╗██║╚════╝██║     ██║██║╚██╗██║██╔══╝  \n"
			 "\t ╚██████╔╝██║ ╚████║      ███████╗██║██║ ╚████║███████╗\n"
 			 "\t  ╚═════╝ ╚═╝  ╚═══╝      ╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝\n"
			 "\t         ██████╗  ██████╗ ███╗   ██╗ ██████╗           \n"
			 "\t         ██╔══██╗██╔═══██╗████╗  ██║██╔════╝           \n"
			 "\t         ██████╔╝██║   ██║██╔██╗ ██║██║  ███╗          \n"
			 "\t         ██╔═══╝ ██║   ██║██║╚██╗██║██║   ██║          \n"
			 "\t         ██║     ╚██████╔╝██║ ╚████║╚██████╔╝          \n"
			 "\t         ╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝           \n\n"
			 "\t\t\t\tstarting...\n");
	sleep(3);
}

void print_end_game(){
	printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
			 "\t  ▄████  ▒█████   ▒█████  ▓█████▄     ▄▄▄▄ ▓██   ██▓▓█████ \n"
			 "\t ██▒ ▀█▒▒██▒  ██▒▒██▒  ██▒▒██▀ ██▌   ▓█████▄▒██  ██▒▓█   ▀ \n"
			 "\t▒██░▄▄▄░▒██░  ██▒▒██░  ██▒░██   █▌   ▒██▒ ▄██▒██ ██░▒███   \n"
			 "\t░▓█  ██▓▒██   ██░▒██   ██░░▓█▄   ▌   ▒██░█▀  ░ ▐██▓░▒▓█  ▄ \n"
			 "\t░▒▓███▀▒░ ████▓▒░░ ████▓▒░░▒████▓    ░▓█  ▀█▓░ ██▒▓░░▒████▒\n"
 			 "\t ░▒   ▒ ░ ▒░▒░▒░ ░ ▒░▒░▒░  ▒▒▓  ▒    ░▒▓███▀▒ ██▒▒▒ ░░ ▒░ ░\n"
			 "\t  ░   ░   ░ ▒ ▒░   ░ ▒ ▒░  ░ ▒  ▒    ▒░▒   ░▓██ ░▒░  ░ ░  ░\n"
			 "\t░ ░   ░ ░ ░ ░ ▒  ░ ░ ░ ▒   ░ ░  ░     ░    ░▒ ▒ ░░     ░   \n"
			 "\t      ░     ░ ░      ░ ░     ░        ░     ░ ░        ░  ░\n"
			 "\t                           ░               ░░ ░            \n\n\n\n\n\n\n");
}

void info(short mod){
	initscr();
    start_color();
	curs_set(FALSE);//прячем курсор
	
	//задаём цветовые пары
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
		
    noecho();
	attron(COLOR_PAIR(2));
	mvprintw(3,2,
				"\tIIIII NN   NN FFFFFFF  OOOOO  \n" 
				"\t III  NNN  NN FF      OO   OO \n" 
				"\t III  NN N NN FFFF    OO   OO \n"
				"\t III  NN N NN FFFF    OO   OO \n" 
				"\t III  NN  NNN FF      OO   OO \n" 
				"\tIIIII NN   NN FF       OOOO0  \n");  
    attroff(COLOR_PAIR(2));
    move(10,1);
    printw("\tEXIT: ");
    attron(COLOR_PAIR(1) | A_BOLD);
    printw("ESC\n\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    if (mod == 1){
		printw("\tPAUSE: ");
		attron(COLOR_PAIR(1) | A_BOLD);
		printw("P\n\n");
		attroff(COLOR_PAIR(1) | A_BOLD);
		printw("\t\tplayer1:\tplayer2:\n");
		printw("\tUP:");
		attron(COLOR_PAIR(1) | A_BOLD);
		printw("\t  W\t\t KEY_UP\n");
		attroff(COLOR_PAIR(1) | A_BOLD);
		printw("\tDOWN:");
		attron(COLOR_PAIR(1) | A_BOLD);
		printw("\t  S\t\t KEY_DOWN\n");
		attroff(COLOR_PAIR(1) | A_BOLD);
	}else{
		printw("\tUP: ");
		attron(COLOR_PAIR(1) | A_BOLD);
		printw("KEY_UP\n\n");
		attroff(COLOR_PAIR(1) | A_BOLD);
		printw("\tDOWN: ");
		attron(COLOR_PAIR(1) | A_BOLD);
		printw("KEY_DOWN\n\n");
	}
	printw("\n\n\tPress any key to continium...");
	box(stdscr, 0, 0);
	getch();
	erase();
    refresh();
    endwin();
}
