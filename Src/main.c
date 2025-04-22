/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Jasey Chanders
 * @brief          : Transitions a status LED for an airport train through three stations:
 * 				   : T (terminal), A (Concourse), S (Storage). LEDs blink when
 * 				   : in a station and fade between station brightness levels
 * 				   : when traveling. At any point an emergency button can be pressed
 * 				   : which 'pauses' the train wherever it is and blinks the LED until
 * 				   : the button is released and the train returns to normal operation from
 * 				   : wherever it was paused.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include "stm32f0xx.h"
#include "utilities.h"
#include <stdbool.h>
#include "log.h"
#include "led.h"


/*
 * @Brief   : Main loop
 */
int main(void)
{
	LOG("Main Loop Starting\r\n");

	init_led();
	set_led(ON);

	for(;;);

}
