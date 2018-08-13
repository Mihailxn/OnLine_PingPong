#include <sys/ioctl.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h> 
#include "pong.h"

void offline_play(short *boll, short *v, short *score, short y_play_1, short y_play_2){
	//граница поля игры сверху или снизу
	if (boll[0] == RAZMER_Y || boll[0] == 0){
		v[0]*=-1;
		fbeep(660, 40);
		//Beep(660, 40);
		printf("\a");
	}
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

void error(char *str){
	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
	curs_set(FALSE);
	WINDOW *error_wnd;
	init_pair(1, COLOR_BLACK, COLOR_RED);
	error_wnd = newwin(5, xMax/2, yMax/4, xMax/4);
	wbkgd(error_wnd, COLOR_PAIR(1));
	mvwprintw(error_wnd, 2,1, "\t%s", str);
    wrefresh(error_wnd);
    getch();
    delwin(error_wnd);
}

void enter_nikname(char *nikname){
	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
	curs_set(TRUE);
	echo();
	char str[] = "Enter your nikname";
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
	wbkgd(stdscr, COLOR_PAIR(5) | A_BOLD);
	mvprintw(yMax/3-1,(xMax-strlen(str))/2, str);
    attron(COLOR_PAIR(1));
	box(stdscr, 0, 0);
	attroff(COLOR_PAIR(4));
    refresh();
    move(yMax/3+1,(xMax-strlen(str))/2);
    getnstr(nikname, MAX_NAME_LEN);
    nikname[MAX_NAME_LEN] = 0;
    curs_set(FALSE);
    noecho();
    erase();
    refresh();
}

void loading(char *text_str){
	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
	//init_pair(4, COLOR_CYAN, COLOR_BLACK);
    //init_pair(5, COLOR_BLUE, COLOR_BLACK);
	wbkgd(stdscr, COLOR_PAIR(5) | A_BOLD);
	mvprintw(yMax/3-1,(xMax-strlen(text_str))/2, text_str);
    attron(COLOR_PAIR(4));
	box(stdscr, 0, 0);
	attroff(COLOR_PAIR(4));
    refresh();
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
    printw("\tEXIT:");
    attron(COLOR_PAIR(1) | A_BOLD);
    printw("\tESC\n\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
	switch(mod){
		case 1:
			printw("\tUP:");
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("\tKEY_UP\n\n");
			attroff(COLOR_PAIR(1) | A_BOLD);
			printw("\tDOWN:");
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("\tKEY_DOWN\n\n");
			attroff(COLOR_PAIR(1) | A_BOLD);
			break;
		case 2:	
			printw("\tPAUSE:");
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("\tP\n\n");
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
			break;
		case 3:
			printw("\tPAUSE:");
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("\tP\n\n");
			attroff(COLOR_PAIR(1) | A_BOLD);
			printw("\tUP:");
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("\tKEY_UP\n");
			attroff(COLOR_PAIR(1) | A_BOLD);
			printw("\tDOWN:");
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("\tKEY_DOWN\n");
			attroff(COLOR_PAIR(1) | A_BOLD);
			break;
		default:
			error("This info mode not yet implemented");
			erase();
			refresh();
			endwin();
			return;
	}
	printw("\n\tScore %d points to win!", SCORE_END);
	printw("\n\n\tPress any key to continium...");
	box(stdscr, 0, 0);
	getch();
	erase();
    refresh();
    endwin();
}

void result_game(short mod){
	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
	init_pair(6, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    switch(mod){
		case 1:
			wbkgd(stdscr, COLOR_PAIR(6) | A_BOLD);
			mvprintw(yMax/3,0,
				"\tYY   YY  OOOOO  UU   UU    WW      WW IIIII NN   NN\n" 
				"\tYY   YY OO   OO UU   UU    WW      WW  III  NNN  NN\n" 
				"\t YYYYY  OO   OO UU   UU    WW   W  WW  III  NN N NN\n" 
				"\t  YYY   OO   OO UU   UU     WW WWW WW  III  NN  NNN\n" 
				"\t  YYY    OOOO0   UUUUU       WW   WW  IIIII NN   NN\n");  
			break;
		case 2:
			wbkgd(stdscr, COLOR_PAIR(7) | A_BOLD);
			mvprintw(yMax/3,0,
				"\tYY   YY  OOOOO  UU   UU    LL       OOOOO   SSSSS   SSSSS \n" 
				"\tYY   YY OO   OO UU   UU    LL      OO   OO SS      SS     \n" 
				"\t YYYYY  OO   OO UU   UU    LL      OO   OO  SSSSS   SSSSS \n" 
				"\t  YYY   OO   OO UU   UU    LL      OO   OO      SS      SS\n" 
				"\t  YYY    OOOO0   UUUUU     LLLLLLL  OOOO0   SSSSS   SSSSS \n");  
			break;
		default:
			error("result mode error");
			break;
    }
	mvprintw(yMax/3+7,(xMax-28)/2,"Press any key to continium...");
	attron(COLOR_PAIR(4));
	box(stdscr, 0, 0);
	attroff(COLOR_PAIR(4) | A_BOLD);
	getch();
	erase();
    refresh();
}

void result_multi_offline_game(char* nikname){
	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
	init_pair(6, COLOR_GREEN, COLOR_BLACK);
    wbkgd(stdscr, COLOR_PAIR(6) | A_BOLD);
	mvprintw(yMax/3-2, 25, nikname);
	mvprintw(yMax/3,0,
			"\t\tWW      WW IIIII NN   NN\n" 
			"\t\tWW      WW  III  NNN  NN\n" 
			"\t\tWW   W  WW  III  NN N NN\n" 
			"\t\t WW WWW WW  III  NN  NNN\n" 
			"\t\t  WW   WW  IIIII NN   NN\n");  
    mvprintw(yMax/3+7,(xMax-28)/2,"Press any key to continium...");
	attron(COLOR_PAIR(4));
	box(stdscr, 0, 0);
	attroff(COLOR_PAIR(4) | A_BOLD);
	getch();
	erase();
    refresh();
}

int getCountsOfDigits(int number) {
	int count = (number == 0) ? 1 : 0;
	while (number != 0) {
		count++;
		number /= 10;
	}
	return count;
}
