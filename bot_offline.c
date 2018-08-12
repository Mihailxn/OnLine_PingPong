#include <stdlib.h>
#include "pong.h"

#define WIN_RATE 70//шанс что бот НЕ пропустит шарик 

short prediction(short *boll, short *v, short position){
	short vector_x = v[1];
	short vector_y = v[0];
	short after_x = boll[1];
	short after_y = boll[0];
	if(position == LEFT){
		while(after_x >= 0){
			after_x += vector_x;
			if((after_y == RAZMER_Y) || (after_y == 0))
				vector_y *= -1;
			after_y += vector_y;
		}
		return after_y;
	}
	if(position == RIGHT){
		while(after_x <= RAZMER_X){
			after_x += vector_x;
			if((after_y == RAZMER_Y) || (after_y == 0))
				vector_y *= -1;
			after_y += vector_y;
		}
		return after_y;
	}
	return -1;
}

short bot(short *boll, short *v, short position, short player_y){
	short target_y;
	short status_bot = 0;
	if(position == LEFT){
		if(boll[1] < RAZMER_X/2){
			if((boll[1] == RAZMER_X/2-1) && (v[1] < 0)){
				if(rand()%100 > WIN_RATE)
					target_y = rand()%RAZMER_Y;
				else target_y = prediction(boll, v, position);
				status_bot = 1;
			}
			if(status_bot){
				if(player_y > target_y)
					return -1;
				if(player_y < target_y)
					return +1;
				if(player_y == target_y){
					status_bot = 0;
					return 0;
				}
			}
		}
	}
	if(position == RIGHT){
		if(boll[1] > RAZMER_X/2){
			if((boll[1] == RAZMER_X/2+1) && (v[1] > 0)){
				if(rand()%100 > WIN_RATE)
					target_y = rand()%RAZMER_Y;
				else target_y = prediction(boll, v, position);
				status_bot = 1;
			}
			if(status_bot){
				if(player_y > target_y)
					return -1;
				if(player_y < target_y)
					return +1;
				if(player_y == target_y){
					status_bot = 0;
					return 0;
				}
			}
		}
	}
	return 0;
}
