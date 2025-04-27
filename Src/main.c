#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "display.h"
#include "led.h"
#include "snake_list.h"
#include "switch.h"
#include "snake_list_tests.h"


int main(void) {

	init_display();
    init_led();
    init_switch();

    LOG("Starting\r\n");
    uint8_t flags;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t prev_x = 0;
    uint8_t prev_y = 0;
    int direction = UP;

   // tests
    test_snake_list();

    Node * snake_head = NULL;
    snake_head = NULL;
    int length_of_snake = 15;
    while(1){

    	flags = get_flags();
    	if(flags) {
    		if (flags & 0x01 && direction != DOWN){ //Up switch
    			direction = UP;
    		} else if (flags & 0x02 && direction != LEFT){ //Right switch
    			direction = RIGHT;
    		} else if (flags & 0x04 && direction != UP){ //Down switch
    			direction = DOWN;
    		} else if (flags & 0x08 && direction != RIGHT){ //Left switch
    			direction = LEFT;
    		}
    		clear_flags();
    	}

    	if(direction == UP && y < SCREEN_WIDTH -1){
    		y +=1;
    		snake_head = add_to_snake_head(x, y);
    	} else if(direction == RIGHT && x < SCREEN_HEIGHT -1){
    		x +=1;
    		snake_head = add_to_snake_head(x, y);
    	} else if(direction == DOWN && y > 1){
    	    y -=1;
    	    snake_head = add_to_snake_head(x, y);
    	} else if(direction == LEFT && x > 1){
    		x -=1;
    		snake_head = add_to_snake_head(x, y);
    	}

    	if(length_of_snake_list() > length_of_snake){
    		remove_from_snake_tail(&prev_x, &prev_y);
    	}
		set_pixel(snake_head->x, snake_head->y, 1);
    	set_pixel(prev_x, prev_y, 0);
		push_display();

    }

	delete_snake_list();
    free(snake_head);



}
