/**
 ******************************************************************************
 * @file           : display.c
 * @author         : Jasey Chanders
 * @brief          : Control for a small SSD1306 OLED display. Allows for x,y
 * 				   : setting of a single pixel
 * @References 	   : This heavily based on the SFE_MicroOLED.h arduino library
 * 				   : from SparkFun Electronics as well as Embedded Systems
 * 				   : Fundamentals by ALexander G. Dean. ChatGPT was also consulted
 * @date           : April 27, 2025
 ******************************************************************************
 */


#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "delay.h"
#include "display.h"

//Control Pins
#define SS_LOW_MSK (GPIO_BSRR_BR_15)
#define SS_HIGH_MSK (GPIO_BSRR_BS_15)
#define DC_LOW_MSK (GPIO_BSRR_BR_9)
#define DC_HIGH_MSK (GPIO_BSRR_BS_9)
#define RST_LOW_MSK (GPIO_BSRR_BR_7)
#define RST_HIGH_MSK (GPIO_BSRR_BS_7)

#define ON 1
#define OFF 0

#define EMPTY_BYTE 0x00

//Screen Dims
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48
#define SCREEN_ROWS 6
#define ROW_HEIGHT 8

//Boot commands
#define SET_CONTRAST 			0x81
#define CONTRAST_VAL 			0x8f
#define DISPLAY_ALL_ON_RESUME 	0xA4
#define NORMAL_DISPLAY 			0xA6
#define NO_OFFSET 				0x00
#define DISPLAY_OFF 			0xAE
#define DISPLAY_ON 				0xAF
#define SET_DISPLAY_OFFSET 		0xD3
#define SET_COMPINS 			0xDA
#define COMPINS_VAL 			0x12
#define SET_VCOMDE_SELECT		0xDB
#define VCOMDE_SELECT_VAL 		0x40
#define SET_DISPLAY_CLOCK_DIV 	0xD5
#define CLOCK_DIV_VAL	    	0x80
#define SET_PRE_CHARGE 			0xD9
#define PRE_CHARGE_VAL 			0xf1
#define SET_MULTIPLEX 			0xA8
#define MULTIPLEX_VAL			0x2f
#define SET_STARTLINE 			0x40
#define STARTLINE_VAL 			0x00
#define COMS_CANDEC 			0xC8
#define SEGRE_MAP 				0xA0
#define SEGRE_MAP_VAL 			0x01
#define CHARGE_PUMP 			0x8D
#define CHARGE_PUMP_VAL			0x14

//Screen update commands
#define SET_ROW_CMD				0xb0
#define SET_COL_CMD				0x10
#define COL_CMD_OFFSET			0x02

#define LAST_NIBBLE_MSK			0x0f

//Memory Layout
//Page buffer 64 x 48 divided by 8 = 384 bytes

	/* LCD Memory organised in 64 horizontal pixel and 6 rows of byte
	 B  B .............B  -----
	 y  y .............y        \
	 t  t .............t         \
	 e  e .............e          \
	 0  1 .............63          \
	                                \
	 D0 D0.............D0            \
	 D1 D1.............D1            / ROW 0
	 D2 D2.............D2           /
	 D3 D3.............D3          /
	 D4 D4.............D4         /
	 D5 D5.............D5        /
	 D6 D6.............D6       /
	 D7 D7.............D7  ----
	*/


// Empty Screen Buffer
static uint8_t screen_memory [] = {


	// ROW0, BYTE0 to BYTE63
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW1, BYTE64 to BYTE127
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW2, BYTE128 to BYTE191
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW3, BYTE192 to BYTE255
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW4, BYTE256 to BYTE319
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW5, BYTE320 to BYTE383
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
 * @Breif      : Initializes SPI and associated GPIO pins
 *
 * @References : Embedded Systems Fundamentals by Alexander Dean Listing 8.5
 * 			   : https://blog.embeddedexpert.io/?p=1650
 */
void init_hardware(void) {


	// Clock gating for SPI1 and GPIO A and B
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

	//CS
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER15, ESF_GPIO_MODER_OUTPUT);

	// GPIO B pin 3, 4, 5 in alternate function 0 (SPI1) for SCK, MISO, MOSI
	// Set each mode field to 2 for alternate function
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, 2);
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER4, 2);
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER5, 2);
	// Select SPI1 (AF = 0) for alternate function
	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL3, 0);
	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL4, 0);
	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL5, 0);

	//DC
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER9,
	ESF_GPIO_MODER_OUTPUT);

	//RST
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER7,
	ESF_GPIO_MODER_OUTPUT);

	// Disable SPI
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 0);


	// Clock is divided by 16 (2^(BR+1))
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR, 7);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1); // Master mode
	// Select first edge sample, active high clock
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 0);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 0);

	// Data is LSB first
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 0);
	// Data is 8 bits long
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_DS, 7);
	// RXNE when at least 1 byte in RX FIFO
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_FRXTH, 1);

	/*Select software slave management by
	 * setting SSM=1 and SSI=1*/
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SSM, 1);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SSI, 1);


	// Enable SPI
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);

}

/*
 * @Breif      : Sends and receives a single byte of data over SPI
 *
 * @References : Embedded Systems Fundamentals by Alexander Dean Listing 8.4
 *
 * @param send : The byte to send
 *
 * @return     : The byte received
 */
uint8_t SPI_transfer(uint8_t send) {
	uint8_t receive = 0;
	// Wait until transmitter buffer is empty
	while ((SPI1->SR & SPI_SR_TXE) == 0)
		 ;
	// Transmit d_ out
	// Must tell compiler to use a byte write (not half- word)
	// by casting SPI1- >DR into a pointer to a byte (uint8_ t).
	// See STM32F0 Snippets (SPI_ 01_ FullDuplexCommunications).
	*((uint8_t *)&(SPI1->DR)) = send;
	// Wait until receiver is not empty
	while ((SPI1->SR & SPI_SR_RXNE) == 0)
		 ;
	// Get d_ in
	receive = (uint8_t) SPI1->DR;
	//LOG("Received %i\r\n", d_in);
	return receive;
};

/*
 * @Breif      : Sends a command over SPI
 *
 * @References : SparkFun Electronics SFE_MicroOLED.h arduino library
 *
 * @param val : The command to send
 */
void send_command(uint8_t val) {

	GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
	SPI_transfer(val);			// Transfer the command byte// Transfer the command byte
	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
}

/*
 * @Breif      : Sends a byte of data over SPI
 *
 * @References : SparkFun Electronics SFE_MicroOLED.h arduino library
 *
 * @param val : The data to send
 */
void send_data(uint8_t val) {

	GPIOA->BSRR |= DC_HIGH_MSK; // DC pin LOW for a command
	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
	SPI_transfer(val);			// Transfer the command byte// Transfer the command byte
	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
}

/* @brief         : Send page address command and address to the SSD1306 OLED controller.
 *
 * @References    : SparkFun Electronics SFE_MicroOLED.h arduino library
 *
 * @param address : The address of the row
*/
void set_row(uint8_t address) {
	address=SET_ROW_CMD|address;
	send_command(address);
	return;
}


/* @brief         : Send column address command and address to the SSD1306 OLED controller.
 *
 * @References    : SparkFun Electronics SFE_MicroOLED.h arduino library
 *
 * @param address : The address of the column
*/
void set_column(uint8_t address) {
	send_command((SET_COL_CMD|(address>>4))+COL_CMD_OFFSET);
	send_command((LAST_NIBBLE_MSK&address));
	return;
}


/*
 * @Breif       : SSD1306 OLED Boot up sequence
 *
 * @References    : SparkFun Electronics SFE_MicroOLED.h arduino library
 */
void start_display()
{
	// Display reset routine
	GPIOA->BSRR |= RST_HIGH_MSK;	// Initially set RST HIGH
	delay_ms(5);	// VDD (3.3V) goes high at start, lets just chill for 5 ms
	GPIOA->BSRR |= RST_LOW_MSK;	// Bring RST low, reset the display
	delay_ms(10);	// wait 10ms
	GPIOA->BSRR |= RST_HIGH_MSK;	// Set RST HIGH, bring out of reset

	// Display Init sequence for 64x48 OLED module
	send_command(DISPLAY_OFF);			// 0xAE

	send_command(SET_DISPLAY_CLOCK_DIV);	// 0xD5
	send_command(CLOCK_DIV_VAL);					// the suggested ratio 0x80

	send_command(SET_MULTIPLEX);			// 0xA8
	send_command(MULTIPLEX_VAL);

	send_command(SET_DISPLAY_OFFSET);		// 0xD3
	send_command(NO_OFFSET);					// no offset

	send_command(SET_STARTLINE | STARTLINE_VAL);	// line #0

	send_command(CHARGE_PUMP);			// enable charge pump
	send_command(CHARGE_PUMP_VAL);

	send_command(NORMAL_DISPLAY);			// 0xA6
	send_command(DISPLAY_ALL_ON_RESUME);	// 0xA4

	send_command(SEGRE_MAP | SEGRE_MAP_VAL);
	send_command(COMS_CANDEC);

	send_command(SET_COMPINS);			// 0xDA
	send_command(COMPINS_VAL);

	send_command(SET_CONTRAST);			// 0x81
	send_command(CONTRAST_VAL);

	send_command(SET_PRE_CHARGE);			// 0xd9
	send_command(PRE_CHARGE_VAL);

	send_command(SET_VCOMDE_SELECT);			// 0xDB
	send_command(VCOMDE_SELECT_VAL);

	send_command(DISPLAY_ON);				//--turn on oled panel
}

/* PUBLIC FUNCTIONS */

//See header file for details
void init_display(void){
	init_hardware();
	start_display();
	clear_display();
	push_display();
}

//See header file for details
void clear_display() {

	for (int i=0;i<SCREEN_ROWS; i++) {
		set_row(i);
		set_column(0);
		for (int j=0; j<SCREEN_WIDTH; j++) {
			screen_memory[i*SCREEN_WIDTH+j] = EMPTY_BYTE;
		}
	}
	push_display();
}


//See header file for details
void push_display(void) {
	uint8_t i, j;

	for (i=0; i<SCREEN_ROWS; i++) {
		set_row(i);
		set_column(0);
		for (j=0;j<SCREEN_WIDTH;j++) {
			send_data(screen_memory[i*SCREEN_WIDTH+j]);
		}
	}
}

//See header file for details
void set_pixel(uint8_t y, uint8_t x, uint8_t value){

	if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
		uint8_t row = y / ROW_HEIGHT;
		uint8_t pixel_idx = y % ROW_HEIGHT;
		uint8_t pixel_msk = (1 << pixel_idx);
		if(value == ON){
			screen_memory[row*SCREEN_WIDTH+x] |= pixel_msk;
		} else {
			screen_memory[row*SCREEN_WIDTH+x] &= ~pixel_msk;
		}
	}else {
		LOG("Pixel out of range\r\n");
	}
}
