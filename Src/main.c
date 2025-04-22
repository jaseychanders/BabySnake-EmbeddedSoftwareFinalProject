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
#include "switch.h"
/*
 * Pinout
 *
      MicroOLED ------------- STM32F091RC
      GND ------------------- GND
      VDD ------------------- 3.3V (VCC)
    D1/MOSI ----------------- D11
    D0/SCK ------------------ D13 (don't change)
      D2
      D/C ------------------- D8 (can be any digital pin)
      RST ------------------- D9 (can be any digital pin)
      CS  ------------------- D10 (can be any digital pin)
 */

void Init_SPI1(void) {
	// Clock gating for SPI1 and GPIO A and B
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	// GPIO A pin 15 in alternate function 0 (SPI1) for NSS
	// Set mode field to 2 for alternate function
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER15, ESF_GPIO_MODER_ALT_FUNC);
	// Select SPI1 (AF = 0) for alternate function
	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRH_AFSEL15, 0);
	// GPIO B pin 3, 4, 5 in alternate function 0 (SPI1) for SCK, MISO, MOSI
	// Set each mode field to 2 for alternate function
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER5, 2);
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER6, 2);
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER7, 2);
	// Select SPI1 (AF = 0) for alternate function
	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL5, 0);
	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL6, 0);
	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL7, 0);
	// Clock is divided by 16 (2^(BR+1))
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR, 3);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1); // Master mode
	// Select first edge sample, active high clock
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 0);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 1);
	// Data is LSB first
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 1);
	// Data is 8 bits long
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_DS, 7);
	// RXNE when at least 1 byte in RX FIFO
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_FRXTH, 1);
	// Have NSS pin asserted automatically
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_NSSP, 1);
	// Enable SPI
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);
}

uint8_t SPI_Send_Receive_Byte(uint8_t d_out) {
	uint8_t d_in;
	// Wait until transmitter buffer is empty
	while ((SPI1->SR & SPI_SR_TXE) == 0)
		 ;
	// Transmit d_ out
	// Must tell compiler to use a byte write (not half- word)
	// by casting SPI1- >DR into a pointer to a byte (uint8_ t).
	// See STM32F0 Snippets (SPI_ 01_ FullDuplexCommunications).
	*((uint8_t *)&(SPI1->DR)) = d_out;
	// Wait until receiver is not empty
	while ((SPI1->SR & SPI_SR_RXNE) == 0)
		 ;
	// Get d_ in
	d_in = (uint8_t) SPI1->DR;
	return d_in;
};

void Test_SPI_Loopback(void) {
	uint8_t out = 'A';
	uint8_t in;
	while (1) {
		in = SPI_Send_Receive_Byte(out);
		// printf("out = %i, in = %i\r\n", out, in);

		 if (in != out){ // Red: error, data does not match
			 set_led(ELED, ON);
		 }else{			 // Green: data matches
			 set_led(ULED, ON);
		 }
			 out++;
		 if (out > 'z')
			 out = 'A';
	}
}

/*
 * @breif   : User switch triggered interrupt
 * 			: Triggers on rising and falling edge of button
 * 			: Code modified from Dean 4.8, 4.9 4.12
 * @param   : void
 * @return  : void
 */
//void EXTI4_15_IRQHandler(void) {
//	//Get g_state of enable/disable for interrupts
//	uint32_t masking_g_state = __get_PRIMASK();
//	__disable_irq();
//	//Check for rising edge
//	if ((EXTI->PR & SWITCH_PIN_MASK) != 0) {
//		EXTI->PR = SWITCH_PIN_MASK; // clear pending request
//		if (get_switch_state()) {
//			set_led(ELED, ON);
//			printf("button pressed\r\n");
//
//		} else { //Falling edge
//			set_led(ELED, OFF);
//			printf("button released\r\n");
//		}
//	}
//	// Clear all other pending requests for this handler
//	EXTI->PR = 0x0000fff0;
//	//Return to previous g_state of enable/disable for interrupts
//	__set_PRIMASK(masking_g_state);
//}




/*
 * @Brief   : Main loop
 */
int main(void)
{
	LOG("Main Loop Starting\r\n");
//
	init_led();
	Init_SPI1();
	Test_SPI_Loopback();
	//init_switch();

	for(;;);

}
