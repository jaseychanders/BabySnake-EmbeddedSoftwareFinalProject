#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//#define SS_LOW_MSK (GPIO_BSRR_BR_15)
//#define SS_HIGH_MSK (GPIO_BSRR_BS_15)
//
//#define MS_TO_ITERATIONS 3195
//
//
//// Blocking delay
//void delay_ms(int msec){
//
////	LOG("START TIMER %i for starting a %i msec delay\r\n", msec, msec);
//
//	int iterations = msec * MS_TO_ITERATIONS;
//	while (iterations-- != 0) { //spins desired num clock cycles
//		__asm volatile("NOP");
//	}
//}
//
//
//void gpio_init(void) {
//
//
//	// Clock gating for SPI1 and GPIO A and B
//	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
//
//	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER15, ESF_GPIO_MODER_OUTPUT);
//
//
//	// GPIO B pin 3, 4, 5 in alternate function 0 (SPI1) for SCK, MISO, MOSI
//	// Set each mode field to 2 for alternate function
//	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, 2);
//	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER4, 2);
//	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER5, 2);
//	// Select SPI1 (AF = 0) for alternate function
//	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL3, 0);
//	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL4, 0);
//	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL5, 0);
//
//	// Disable SPI
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 0);
//
//
//	// Clock is divided by 16 (2^(BR+1))
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR, 7);
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1); // Master mode
//	// Select first edge sample, active high clock
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 1);
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 1);
//
//	// Data is LSB first
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 0);
//	// Data is 8 bits long
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_DS, 7);
//	// RXNE when at least 1 byte in RX FIFO
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_FRXTH, 1);
//
//	/*Select software slave management by
//	 * setting SSM=1 and SSI=1*/
//	SPI1->CR1 |= (1<<8);
//	SPI1->CR1 |= (1<<9);
//
//	// Enable SPI
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);
//
//}
//
//
//uint8_t SPI_Send_Receive_Byte(uint8_t d_out) {
////	LOG("Send %i\r\n", d_out);
//	uint8_t d_in = 236;
//	// Wait until transmitter buffer is empty
//	while ((SPI1->SR & SPI_SR_TXE) == 0)
//		 ;
//	// Transmit d_ out
//	// Must tell compiler to use a byte write (not half- word)
//	// by casting SPI1- >DR into a pointer to a byte (uint8_ t).
//	// See STM32F0 Snippets (SPI_ 01_ FullDuplexCommunications).
//	*((uint8_t *)&(SPI1->DR)) = d_out;
//	// Wait until receiver is not empty
//	while ((SPI1->SR & SPI_SR_RXNE) == 0)
//		 ;
//	// Get d_ in
//	d_in = (uint8_t) SPI1->DR;
//	//LOG("Received %i\r\n", d_in);
//	return d_in;
//};
//
//void write(uint8_t reg, uint8_t val) {
////	LOG("Send command %i\r\n", c);
//
//	//GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
//	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
//	//delay_ms(1);
//	SPI_Send_Receive_Byte((reg & 0x3F));
//	SPI_Send_Receive_Byte(val);			// Transfer the command byte// Transfer the command byte
//	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
//}
//
//uint8_t read(uint8_t reg) {
////	LOG("Send command %i\r\n", c);
//
//	//GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
//	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
//	//delay_ms(1);
//	SPI_Send_Receive_Byte(0x80 | (reg & 0x3F));
//	uint8_t d_in = SPI_Send_Receive_Byte(0x00);			// Transfer the command byte// Transfer the command byte
//	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
//	return d_in;
//}


//int main(void) {
//    // Initialize GPIO, SPI and OLED
//    gpio_init();
//    write(0x20, 0x57); //enable reads
//
//    while(1){
//    	uint8_t low = read(0x28);
//    	uint8_t high = read(0x29);
//    	int16_t x = (int16_t)(high << 8 | low) >> 4;
//    	printf("%i\r\n", x);
//    	delay_ms(1);
//
//    }
//}
