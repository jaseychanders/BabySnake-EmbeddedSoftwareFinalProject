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
    while(1){

    	flags = get_flags();
    	if(flags) {
    		if (flags & 0x01){ //Up switch
    			set_led(ELED, get_switch_state(UP));
    		} else if (flags & 0x02){ //Right switch
    			set_led(ULED, get_switch_state(RIGHT));
    		} else if (flags & 0x04){ //Down switch
    			set_led(ELED, get_switch_state(DOWN));
    		} else if (flags & 0x08){ //Left switch
    			set_led(ULED, get_switch_state(LEFT));
    		}
    		clear_flags();
    	}


//    	uint8_t prev_x = 0;
//    	uint8_t prev_y = 0;
//    	for(int i = 0; i < 48; i++){
//    		for(int j = 0; j < 64; j++){
//    			set_pixel(prev_x, prev_y, 0);
//    			set_pixel(i, j, 1);
//    			push_display();
//    			prev_x = i;
//    			prev_y = j;
//    			//delay_ms(1);
//    		}
//    	}

    }

}
