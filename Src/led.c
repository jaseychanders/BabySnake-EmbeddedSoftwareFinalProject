/**
 ******************************************************************************
 * @file           : led.c
 * @author         : Jasey Chanders
 * @brief          : Allows for simple access to User LED and an external LED
 * @date           : Feb 16, 2025
 ******************************************************************************
 */

#include "led.h"

//User LED on PA5
#define ULED_OFF_MSK (GPIO_BSRR_BR_5)
#define ULED_ON_MSK (GPIO_BSRR_BS_5)


/*
 * @Brief   : Initializes LED
 * 			: from dean book 7.15
 *
 * @params  : void
 * @return  : void
 */

void init_led(){

	//User LED
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER5,
	ESF_GPIO_MODER_OUTPUT);

}

/*
 * @Brief        : Set LED
 *
 * @param led_id : the LED to set
 * @param state  : The value for the led (anything > 0 results in on for ULED
 *
 * @return 		 : void
 *
 */
void set_led(int state){

	if (state > 0) { // Turn LED on
		GPIOA->BSRR |= ULED_ON_MSK;
	} else { // Turn LED off
			GPIOA->BSRR |= ULED_OFF_MSK;
	}
}
