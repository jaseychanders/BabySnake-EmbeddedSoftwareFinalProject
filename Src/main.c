/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Jasey Chanders
 * @brief          : Runs tests and then begins the game, updates the state machine
 * @date           : April 30th, 2025
 ******************************************************************************
 */

#include "stm32f0xx.h"
#include "utilities.h"
#include "log.h"
#include "delay.h"
#include "snake_list_tests.h"
#include "state_machine.h"
#include "display.h"
#include "state_machine_tests.h"
#include "switches.h"

#define UPDATE_FREQUENCY 50 //How long to delay between state machine updates, controls speed of the game

int main(void) {

	// Initialization
	init_display();
    init_switches();

    LOG("Starting Snake\r\n");


    // Tests
    test_snake_list();
    test_state_machine();

    // Start the game
    start_game();

    // Call the state machine update infinitely
    while(1){
    	update_sate_machine();
    	delay_ms(UPDATE_FREQUENCY);
    }


}
