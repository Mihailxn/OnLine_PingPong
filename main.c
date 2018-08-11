#include <stdlib.h>
#include "pong.h"

int main(int argc, char ** argv){
	print_load_game();
	short highlight = 0;
	while(1){
		if ((highlight= menu(highlight))==-1) break;
		switch(highlight){
			case 0: info(1); pong(1); break;
			case 1: info(2); pong(2); break;
			case 2: info(3); pong(3); break;
			default: break;
		}
	}	
	print_end_game();
}
