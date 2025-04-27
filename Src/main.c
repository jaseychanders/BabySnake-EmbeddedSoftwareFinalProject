#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "display.h"
#include "led.h"
#include "switch.h"


int main(void) {
    init_display();
    init_led();
    init_switch();
    uint8_t flags;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t prev_x = 0;
    uint8_t prev_y = 0;
    int direction = UP;
    while(1){

    	flags = get_flags();
    	if(flags) {
    		if (flags & 0x01){ //Up switch
    			LOG("UP\r\n");
    			direction = UP;
    		} else if (flags & 0x02){ //Right switch
    			LOG("RIGHT\r\n");
    			direction = RIGHT;
    		} else if (flags & 0x04){ //Down switch
    			LOG("DOWN\r\n");
    			direction = DOWN;
    		} else if (flags & 0x08){ //Left switch
    			LOG("LEFT\r\n");
    			direction = LEFT;
    		}
    		clear_flags();
    	}

    	if(direction == UP && y < SCREEN_WIDTH -1){
    		y +=1;
    	} else if(direction == RIGHT && x < SCREEN_HEIGHT -1){
    		x +=1;
    	} else if(direction == DOWN && y > 1){
    	    y -=1;
    	} else if(direction == LEFT && x > 1){
    		x -=1;
    	}

    	set_pixel(prev_x, prev_y, 0);
		set_pixel(x, y, 1);
		prev_x = x;
		prev_y = y;
		push_display();




//    	for(int i = 0; i < 48; i++){
//    		for(int j = 0; j < 64; j++){
//
//
//    			//delay_ms(1);
//    		}
//    	}

    }

}
