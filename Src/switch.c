/**
 ******************************************************************************
 * @file           : switch.c
 * @author         : Jasey Chanders
 * @brief          : Allows for simple access to User switch
 * @date           : Mar 1, 2025
 ******************************************************************************
 */

#include "switch.h"

static volatile uint8_t flags = 0x00;

//Init switch with interrupt support
//code from Dean 4.8, 4.9 4.12
void init_switch(){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER12, ESF_GPIO_MODER_INPUT);
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER8, ESF_GPIO_MODER_INPUT);
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER6, ESF_GPIO_MODER_INPUT);
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER10, ESF_GPIO_MODER_INPUT);

	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR12, 1);
	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR8, 1);
	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR6, 1);
	MODIFY_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR10, 1);


	// Enable peripheral clock for SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	// Select Port C for bits for SW1 and SW2
	// SW1 is at Port C bit 13
	MODIFY_FIELD(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI12, 0);
	MODIFY_FIELD(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI8, 0);
	MODIFY_FIELD(SYSCFG->EXTICR[1], SYSCFG_EXTICR2_EXTI6, 0);
	MODIFY_FIELD(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI10, 0);

	// Set mask bits for inputs in EXTI_ IMR
	EXTI->IMR |= GPIO_IDR_12;
	EXTI->IMR |= GPIO_IDR_8;
	EXTI->IMR |= GPIO_IDR_6;
	EXTI->IMR |= GPIO_IDR_10;

	// Trigger on both rising and falling edges in EXTI_ RTSR and EXTI_ FTSR
	EXTI->RTSR |= GPIO_IDR_12;
	EXTI->RTSR |= GPIO_IDR_8;
	EXTI->RTSR |= GPIO_IDR_6;
	EXTI->RTSR |= GPIO_IDR_10;

	EXTI->FTSR |= GPIO_IDR_12;
	EXTI->FTSR |= GPIO_IDR_8;
	EXTI->FTSR |= GPIO_IDR_6;
	EXTI->FTSR |= GPIO_IDR_10;

	// Configure enable and mask bits for NVIC IRQ Channel for EXTI
	// Interrupt lines 7 and 13 are both serviced by EXTI4_ 15_ IRQ
	NVIC_SetPriority(EXTI4_15_IRQn, 3);
	NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	// Optional: Configure PRIMASK in case interrupts were disabled
	__enable_irq();
}


//Check switch - from slides
bool get_switch_state(int switch_ID)
{
	switch(switch_ID){
		case UP:
			return !(GPIOA->IDR & GPIO_IDR_12);
		case RIGHT:
			return !(GPIOA->IDR & GPIO_IDR_10);
		case DOWN:
			return !(GPIOA->IDR & GPIO_IDR_6);
		case LEFT:
			return !(GPIOA->IDR & GPIO_IDR_8);
		default:
			LOG("No such switch id\r\n");
			return 0;
	}

}


/*
 * @breif   : User switch triggered interrupt
 * 			: Triggers on rising and falling edge of button
 * 			: Code modified from Dean 4.8, 4.9 4.12
 * @param   : void
 * @return  : void
 *
 * sometimes triggers multiple reads
 */
void EXTI4_15_IRQHandler(void) {
	//Get g_state of enable/disable for interrupts
	uint32_t masking_g_state = __get_PRIMASK();
	__disable_irq();
	//Check for rising edge
	if ((EXTI->PR & GPIO_IDR_12) != 0) {
		EXTI->PR = GPIO_IDR_12; // clear pending request
		if(!(GPIOA->IDR & GPIO_IDR_12)){
			flags |= 0x01;
		}

	} else if ((EXTI->PR & GPIO_IDR_10) != 0) {
		EXTI->PR = GPIO_IDR_10; // clear pending request
		if(!(GPIOA->IDR & GPIO_IDR_10)){
			flags |= 0x02;
		}
	}else if ((EXTI->PR & GPIO_IDR_6) != 0) {
		EXTI->PR = GPIO_IDR_6; // clear pending request
		if(!(GPIOA->IDR & GPIO_IDR_6)){
			flags |= 0x04;
		}
	} else if ((EXTI->PR & GPIO_IDR_8) != 0) {
		EXTI->PR = GPIO_IDR_8; // clear pending request
		if(!(GPIOA->IDR & GPIO_IDR_8)){
			flags |= 0x08;
		}
	}
	// Clear all other pending requests for this handler
	EXTI->PR = 0x00000000;
	//Return to previous g_state of enable/disable for interrupts
	__set_PRIMASK(masking_g_state);
}

void clear_flags(){
	flags = 0x00;
}

uint8_t get_flags(){
	return flags;
}

