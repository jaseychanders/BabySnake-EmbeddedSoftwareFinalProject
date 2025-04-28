#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "snake_list_tests.h"
#include "state_machine.h"
#include "display.h"
#include "led.h"
#include "switch.h"
#include "state_machine_tests.h"


int main(void) {

	init_display();
    init_led();
    init_switch();

    LOG("Starting Snake\r\n");


   // tests
    test_snake_list();
    test_state_machine();

    start_game();

    while(1){
    	update_sate_machine();
    	delay_ms(100);
    }


}
