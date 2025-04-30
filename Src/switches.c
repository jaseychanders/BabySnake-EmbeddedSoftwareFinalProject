/**
 ******************************************************************************
 * @file           : switch.h
 * @author         : Jasey Chanders
 * @brief          : Reads 4 external switches using interrupts and updates flags
 * @date           : April 30th, 2025
 ******************************************************************************
 */

#include "switches.h"

#define CLEAR_FLAGS 0x00

/*
 * Switch flags
 * Each switch is represented by a single bit
 * 0x01	 => 	Up switch
 * 0x02  => 	Right switch
 * 0x04  => 	Down switch
 * 0x08  => 	Left switch
 */
static volatile uint8_t flags = CLEAR_FLAGS;


// See Header file for details
void init_switches(){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;										// Enable Port A

	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER12, ESF_GPIO_MODER_INPUT);	// Set PA12 to output
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER8, ESF_GPIO_MODER_INPUT);	// Set PA8 to output
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER6, ESF_GPIO_MODER_INPUT);	// Set PA6 to output
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER10, ESF_GPIO_MODER_INPUT);	// Set PA10 to output

	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR12, 1);						// Set pull ups
	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR8, 1);
	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR6, 1);
	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR10, 1);

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;								// Enable peripheral clock for SYSCFG

	MODIFY_FIELD(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI12, 0);				// Select Interrupt lines
	MODIFY_FIELD(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI8, 0);
	MODIFY_FIELD(SYSCFG->EXTICR[1], SYSCFG_EXTICR2_EXTI6, 0);
	MODIFY_FIELD(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI10, 0);

	EXTI->IMR |= GPIO_IDR_12;												// Set mask bits for inputs in EXTI_IMR
	EXTI->IMR |= GPIO_IDR_8;
	EXTI->IMR |= GPIO_IDR_6;
	EXTI->IMR |= GPIO_IDR_10;

	EXTI->RTSR |= GPIO_IDR_12;												// Trigger on rising edges
	EXTI->RTSR |= GPIO_IDR_8;
	EXTI->RTSR |= GPIO_IDR_6;
	EXTI->RTSR |= GPIO_IDR_10;


	NVIC_SetPriority(EXTI4_15_IRQn, 3);										// Configure Interrupt
	NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	__enable_irq();
}

/*
 * @Breif   : Handle interrupt triggered by any of the 4 switched
 * 			: Triggers on rising edge of button
 *
 * @References	: Embedded Systems Fundamentals by Alexander Dean Listings 4.8, 4.9 4.12
 */
void EXTI4_15_IRQHandler(void) {
	uint32_t masking_g_state = __get_PRIMASK();								// Get g_state of enable/disable for interrupts
	__disable_irq();														// Disable Interrupts

	if ((EXTI->PR & GPIO_IDR_12) != 0) {									// Determine which button caused the interrupt
		EXTI->PR = GPIO_IDR_12; 											// Clear pending request
		flags |= UP_SWITCH_MSK;												// Set button pressed flag
	} else if ((EXTI->PR & GPIO_IDR_10) != 0) {
		EXTI->PR = GPIO_IDR_10;
		flags |= RIGHT_SWITCH_MSK;
	}else if ((EXTI->PR & GPIO_IDR_6) != 0) {
		EXTI->PR = GPIO_IDR_6;
		flags |= DOWN_SWITCH_MSK;
	} else if ((EXTI->PR & GPIO_IDR_8) != 0) {
		EXTI->PR = GPIO_IDR_8;
		flags |= LEFT_SWITCH_MSK;
	}

	EXTI->PR = 0x00000000;													// Clear all other pending requests for this handler

	__set_PRIMASK(masking_g_state);											//Return to previous g_state of enable/disable for interrupts
}

// See header file for details
void clear_flags(){
	flags = CLEAR_FLAGS;
}

// See header file for details
uint8_t get_flags(){
	return flags;
}

