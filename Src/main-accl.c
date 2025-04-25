#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// Pin definitions
#define RST_PIN   (1 << 0)   // PA0 (OLED Reset)
#define DC_PIN    (1 << 1)   // PA1 (OLED DC)
#define CS_PIN    (1 << 4)   // PA4 (OLED CS)
#define SCK_PIN   (1 << 5)   // PA5 (SPI1 SCK)
#define MOSI_PIN  (1 << 7)   // PA7 (SPI1 MOSI)

#define SETCONTRAST 		0x81
#define DISPLAYALLONRESUME 	0xA4
#define DISPLAYALLON 		0xA5
#define NORMALDISPLAY 		0xA6
#define INVERTDISPLAY 		0xA7
#define DISPLAYOFF 			0xAE
#define DISPLAYON 			0xAF
#define SETDISPLAYOFFSET 	0xD3
#define SETCOMPINS 			0xDA
#define SETVCOMDESELECT		0xDB
#define SETDISPLAYCLOCKDIV 	0xD5
#define SETPRECHARGE 		0xD9
#define SETMULTIPLEX 		0xA8
#define SETLOWCOLUMN 		0x00
#define SETHIGHCOLUMN 		0x10
#define SETSTARTLINE 		0x40
#define MEMORYMODE 			0x20
#define COMSCANINC 			0xC0
#define COMSCANDEC 			0xC8
#define SEGREMAP 			0xA0
#define CHARGEPUMP 			0x8D
#define EXTERNALVCC 		0x01
#define SWITCHCAPVCC 		0x02
#define ALL					1
#define LCDWIDTH			64
#define LCDHEIGHT			48

#define SS_LOW_MSK (GPIO_BSRR_BR_15)
#define SS_HIGH_MSK (GPIO_BSRR_BS_15)

#define MS_TO_ITERATIONS 3195

/** \brief MicroOLED screen buffer.
//
//Page buffer 64 x 48 divided by 8 = 384 bytes
//Page buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.  This page buffer serves as a scratch RAM for graphical functions.  All drawing function will first be drawn on this page buffer, only upon calling display() function will transfer the page buffer to the actual LCD controller's memory.
//*/



// Blocking delay
void delay_ms(int msec){

//	LOG("START TIMER %i for starting a %i msec delay\r\n", msec, msec);

	int iterations = msec * MS_TO_ITERATIONS;
	while (iterations-- != 0) { //spins desired num clock cycles
		__asm volatile("NOP");
	}
}


void gpio_init(void) {


	// Clock gating for SPI1 and GPIO A and B
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

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

	// Disable SPI
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 0);


	// Clock is divided by 16 (2^(BR+1))
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR, 7);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1); // Master mode
	// Select first edge sample, active high clock
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 1);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 1);

	//MODIFY_FIELD(SPI1->CR1, SPI_CR1_SSM, 0);  // Hardware slave management
	// Data is LSB first
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 0);
	// Data is 8 bits long
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_DS, 7);
	// RXNE when at least 1 byte in RX FIFO
	MODIFY_FIELD(SPI1->CR2, SPI_CR2_FRXTH, 1);
//	// Have NSS pin asserted automatically
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_NSSP, 1);


	/*Select software slave management by
	 * setting SSM=1 and SSI=1*/
	SPI1->CR1 |= (1<<8);
	SPI1->CR1 |= (1<<9);

	// Enable SPI
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);


	// Clock gating for SPI1 and GPIO A and B
//	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
//
//	//Chip Select
//	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER4, ESF_GPIO_MODER_OUTPUT);
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
//	// Clock is divided by 16 (2^(BR+1))
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR, 7);
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1); // Master mode
//	// Select first edge sample, active high clock
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 1);
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 1);
//	// Data is LSB first
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 0); //MSB
//	// Data is 8 bits long
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_DS, 15); // 16 bit
//	// RXNE when at least 1 byte in RX FIFO
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_FRXTH, 1);
//	// Have NSS pin asserted automatically
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_NSSP, 1);
//	// Enable SPI
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);

}
//    // Enable GPIOA clock
//    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//
//    // Configure PA0, PA1, PA4 as output (RST, DC, CS)
//    GPIOA->MODER |= (1 << (0 * 2)) | (1 << (1 * 2)) | (1 << (4 * 2)); // output mode
//    GPIOA->OTYPER &= ~(RST_PIN | DC_PIN | CS_PIN);  // Push-pull
//    GPIOA->OSPEEDR |= (1 << (0 * 2)) | (1 << (1 * 2)) | (1 << (4 * 2));  // High speed
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
////    // Configure PA5 (SCK) and PA7 (MOSI) for SPI (Alternate function)
////    GPIOA->MODER |= (2 << (5 * 2)) | (2 << (7 * 2));  // Alternate function mode
////    GPIOA->AFR[0] |= (0x01 << (5 * 4)) | (0x01 << (7 * 4)); // AF0 for SPI1 (SCK and MOSI)
//}

//void spi_init(void) {
//    // Enable SPI1 clock
//    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//
//    // Configure SPI1 (Mode 0, MSB first, software NSS, baud rate f_PCLK/256)
//    SPI1->CR1 = 0;
//    SPI1->CR1 |= SPI_CR1_MSTR;           // Master mode
//    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Software NSS
//    SPI1->CR1 |= SPI_CR1_CPOL;          // Clock polarity = 0
//    SPI1->CR1 |= SPI_CR1_CPHA;          // Clock phase = 0
//    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;      // MSB first
//    SPI1->CR1 |= (7 << 3);               // Baud rate = f_PCLK/256 (slowest)
//    SPI1->CR1 |= SPI_CR1_SPE;            // Enable SPI1
//}

//void spi_write(uint8_t reg, uint8_t val) {
//    while (!(SPI1->SR & SPI_SR_TXE)); // Wait for transmit buffer to be empty
//    GPIOB->BSRR |= SS_LOW_MSK;
//    reg &= 0x3f;
//    uint16_t transfer = reg << 8;
//    transfer |= val;
//    SPI1->DR = transfer;
//    GPIOB->BSRR |= SS_LOW_MSK;
//    while (!(SPI1->SR & SPI_SR_TXE)); // Wait for transmission to complete
//    while (SPI1->SR & SPI_SR_BSY);    // Wait for SPI to be not busy
//}

uint8_t SPI_Send_Receive_Byte(uint8_t d_out) {
//	LOG("Send %i\r\n", d_out);
	uint8_t d_in = 236;
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
	//LOG("Received %i\r\n", d_in);
	return d_in;
};

void write(uint8_t reg, uint8_t val) {
//	LOG("Send command %i\r\n", c);

	//GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
	//delay_ms(1);
	SPI_Send_Receive_Byte((reg & 0x3F));
	SPI_Send_Receive_Byte(val);			// Transfer the command byte// Transfer the command byte
	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
}

uint8_t read(uint8_t reg) {
//	LOG("Send command %i\r\n", c);

	//GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
	//delay_ms(1);
	SPI_Send_Receive_Byte(0x80 | (reg & 0x3F));
	uint8_t d_in = SPI_Send_Receive_Byte(0x00);			// Transfer the command byte// Transfer the command byte
	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
	return d_in;
}



//uint16_t SPI_Send_Receive_Byte(uint16_t d_out) {
////	LOG("Send %i\r\n", d_out);
//	uint16_t d_in = 0;
//	// Wait until transmitter buffer is empty
//	while ((SPI1->SR & SPI_SR_TXE) == 0)
//		 ;
//
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
////	LOG("Received %i\r\n", d_in);
//	return d_in;
//};
//
//void command(uint8_t reg, uint8_t val) {
////	LOG("Send command %i\r\n", c);
//
////	GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
//	GPIOA->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
//	reg &= 0x3f;
//	uint16_t transfer = reg << 8;
//	transfer |= val;
//	//SPI_Send_Receive_Byte(transfer);			// Transfer the command byte
//	delay_ms(1);
//	GPIOA->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
//	delay_ms(1);
//}
//
//void oled_cmd(uint8_t cmd) {
//    GPIOA->ODR &= ~DC_PIN; // DC LOW for command
//    GPIOA->ODR &= ~CS_PIN; // CS LOW
//    spi_write(cmd);
//    GPIOA->ODR |= CS_PIN;  // CS HIGH
//}
//
//void oled_data(uint8_t data) {
//    GPIOA->ODR |= DC_PIN;  // DC HIGH for data
//    GPIOA->ODR &= ~CS_PIN; // CS LOW
//    spi_write(data);
//    GPIOA->ODR |= CS_PIN;  // CS HIGH
//}
//
//void oled_reset(void) {
//    GPIOA->ODR &= ~RST_PIN; // RST LOW
//    delay_ms(10);
//    GPIOA->ODR |= RST_PIN;  // RST HIGH
//    delay_ms(10);
//}
//
///** \brief Set SSD1306 page address.
//
//    Send page address command and address to the SSD1306 OLED controller.
//*/
//void setPageAddress(uint8_t add) {
//	add=0xb0|add;
//	oled_cmd(add);
//	return;
//}
//
///** \brief Set SSD1306 column address.
//
//    Send column address command and address to the SSD1306 OLED controller.
//*/
//void setColumnAddress(uint8_t add) {
//	oled_cmd((0x10|(add>>4))+0x02);
//	oled_cmd((0x0f&add));
//	return;
//}
//
///** \brief Clear screen buffer or SSD1306's memory.
//
//    To clear GDRAM inside the LCD controller, pass in the variable mode = ALL and to clear screen page buffer pass in the variable mode = PAGE.
//*/
//void clear(uint8_t mode) {
//	//	uint8_t page=6, col=0x40;
//	if (mode==ALL) {
//		for (int i=0;i<8; i++) {
//			setPageAddress(i);
//			setColumnAddress(0);
//			for (int j=0; j<0x80; j++) {
//				oled_data(0);
//			}
//		}
//	}
//	else
//	{
//		memset(screenmemory,0,384);			// (64 x 48) / 8 = 384
//		//display();
//	}
//}
//
///** \brief Transfer display memory.
//
//    Bulk move the screen buffer to the SSD1306 controller's memory so that images/graphics drawn on the screen buffer will be displayed on the OLED.
//*/
//void display(void) {
//	uint8_t i, j;
//
//	for (i=0; i<6; i++) {
//		setPageAddress(i);
//		setColumnAddress(0);
//		for (j=0;j<0x40;j++) {
//			oled_data(screenmemory[i*0x40+j]);
//		}
//	}
//}
//
//void oled_init(void) {
//		oled_reset();
//		// Display Init sequence for 64x48 OLED module
//		oled_cmd(DISPLAYOFF);			// 0xAE
//
//		oled_cmd(SETDISPLAYCLOCKDIV);	// 0xD5
//		oled_cmd(0x80);					// the suggested ratio 0x80
//
//		oled_cmd(SETMULTIPLEX);			// 0xA8
//		oled_cmd(0x2F);
//
//		oled_cmd(SETDISPLAYOFFSET);		// 0xD3
//		oled_cmd(0x0);					// no offset
//
//		oled_cmd(SETSTARTLINE | 0x0);	// line #0
//
//		oled_cmd(CHARGEPUMP);			// enable charge pump
//		oled_cmd(0x14);
//
//		oled_cmd(NORMALDISPLAY);			// 0xA6
//		oled_cmd(DISPLAYALLONRESUME);	// 0xA4
//
//		oled_cmd(SEGREMAP | 0x1);
//		oled_cmd(COMSCANDEC);
//
//		oled_cmd(SETCOMPINS);			// 0xDA
//		oled_cmd(0x12);
//
//		oled_cmd(SETCONTRAST);			// 0x81
//		oled_cmd(0x8F);
//
//		oled_cmd(SETPRECHARGE);			// 0xd9
//		oled_cmd(0xF1);
//
//		oled_cmd(SETVCOMDESELECT);			// 0xDB
//		oled_cmd(0x40);
//
//		oled_cmd(DISPLAYON);				//--turn on oled panel
//		clear(ALL);
//}



int main(void) {
    // Initialize GPIO, SPI and OLED
    gpio_init();
    write(0x20, 0x57); //enable reads
   // spi_init();
   // oled_init();

    while(1){
    	uint8_t low = read(0x28);
    	uint8_t high = read(0x29);
    	int16_t x = (int16_t)(high << 8 | low) >> 4;
    	printf("%i\r\n", x);
    	delay_ms(1);
    	//display();
    	//oled_data(0xaa);
    	//oled_cmd(0x11);
    	//oled_reset();
    	//delay_ms(5);
    }
}
