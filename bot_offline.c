#include <stdlib.h>
#include "pong.h"

#define WIN_RATE 90
#define SHIFT 4

#define SCAN 0
#define RANDOM 1
#define MOVEING 2

short prediction(short *boll, short *v, short position){
	short vector_x = v[1];
	short vector_y = v[0];
	short after_x = boll[1];
	short target_y = boll[0];
	if(position == LEFT){
		while(after_x >= 0){
			after_x += vector_x;
			if((target_y == RAZMER_Y) || (target_y == 0))
				vector_y *= -1;
			target_y += vector_y;
		}
		return target_y;
	}
	if(position == RIGHT){
		while(after_x <= RAZMER_X-1){
			after_x += vector_x;
			if((target_y == RAZMER_Y) || (target_y == 0))
				vector_y *= -1;
			target_y += vector_y;
		}
		return target_y;
	}
	return -1;
}

short bot(short *boll, short *v, short position, short player_y){
	static unsigned time_bot = 0;
	static short status = 0;
	static short target_y = 0;
	if(status == SCAN){
		if(position == LEFT){
			if((boll[1] < RAZMER_X/2) && (v[1] < 0)){
				target_y = prediction(boll, v, position);
				status = RANDOM;
			}
		}
		if(position == RIGHT){
			if((boll[1] > RAZMER_X/2) && (v[1] > 0)){
				target_y = prediction(boll, v, position);
				status = RANDOM;
			}
		}
	}

	if(status == RANDOM){
		if(rand()%100 > WIN_RATE){
            if(rand()%2){
                if((target_y+SHIFT) > RAZMER_Y){
                    target_y-=SHIFT*2;
                }else target_y+=SHIFT;
            }else{
                if((target_y-SHIFT) < 0){
                    target_y+=SHIFT*2;
                }else target_y-=SHIFT;
            }
        }
		status = MOVEING;
	}
	if(!(++time_bot%40)){
		if(status == MOVEING){
			if(position == LEFT){
				if(player_y == target_y){
					if(v[1] < 0)
						status = SCAN;
					return 0;
				}
				if(player_y < target_y)
					return +1;
				if(player_y > target_y)
					return -1;
				
			}
			if(position == RIGHT){
				if(player_y == target_y){
					if(v[1] < 0)
						status = SCAN;
					return 0;
				}
				if(player_y < target_y)
					return +1;
				if(player_y > target_y)
					return -1;
			}
		}
	}
	return 0;
}
