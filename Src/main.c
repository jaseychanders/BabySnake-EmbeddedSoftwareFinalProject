#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "display.h"

int main(void) {
    init_display();
    while(1){
    	uint8_t prev_x = 0;
    	uint8_t prev_y = 0;
    	for(int i = 0; i < 48; i++){
    		for(int j = 0; j < 64; j++){
    			set_pixel(prev_x, prev_y, 0);
    			set_pixel(i, j, 1);
    			push_display();
    			prev_x = i;
    			prev_y = j;
    			//delay_ms(1);
    		}
    	}

    }

}
