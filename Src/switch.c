/**
 ******************************************************************************
 * @file           : switch.c
 * @author         : Jasey Chanders
 * @brief          : Allows for simple access to User switch
 * @date           : Mar 1, 2025
 ******************************************************************************
 */

#include "switch.h"

// User switch on PORT C, pin 13
#define SWITCH_GPIO_PORT GPIOA
#define SWITCH_PIN 3
#define SWITCH_IDR GPIOA->IDR
#define GPIO_PULL_UP_VALUE 1

//Init switch with interrupt support
//code from Dean 4.8, 4.9 4.12
void init_switch(){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	MODIFY_FIELD(SWITCH_GPIO_PORT->MODER, GPIO_MODER_MODER3,
	ESF_GPIO_MODER_INPUT);

	MODIFY_FIELD(SWITCH_GPIO_PORT->PUPDR, GPIO_PUPDR_PUPDR3,
	GPIO_PULL_UP_VALUE);

	// Enable peripheral clock for SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

		// Select Port C for bits for SW1 and SW2 out dated
		// SW1 is at Port C bit 13
	MODIFY_FIELD(SYSCFG->EXTICR[1], SYSCFG_EXTICR1_EXTI3, 2);

	// Set mask bits for inputs in EXTI_ IMR
	EXTI->IMR |= SWITCH_PIN_MASK;
	// Trigger on both rising and falling edges in EXTI_RTSR and EXTI_FTSR
	EXTI->RTSR |= SWITCH_PIN_MASK;
	EXTI->FTSR |= SWITCH_PIN_MASK;
	// Configure enable and mask bits for NVIC IRQ Channel for EXTI
	// Interrupt lines 7 and 13 are both serviced by EXTI4_ 15_ IRQ
	NVIC_SetPriority(EXTI4_15_IRQn, 3);
	NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	// Optional: Configure PRIMASK in case interrupts were disabled
	__enable_irq();
}


//Check switch - from slides
bool get_switch_state()
{
	return !(SWITCH_IDR & SWITCH_PIN_MASK);
}
