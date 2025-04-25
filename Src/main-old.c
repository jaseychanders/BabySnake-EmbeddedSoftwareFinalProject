#include "stm32f0xx.h"
#include <stdio.h>

//#define SETCONTRAST 		0x81
//#define DISPLAYALLONRESUME 	0xA4
//#define DISPLAYALLON 		0xA5
//#define NORMALDISPLAY 		0xA6
//#define INVERTDISPLAY 		0xA7
//#define DISPLAYOFF 			0xAE
//#define DISPLAYON 			0xAF
//#define SETDISPLAYOFFSET 	0xD3
//#define SETCOMPINS 			0xDA
//#define SETVCOMDESELECT		0xDB
//#define SETDISPLAYCLOCKDIV 	0xD5
//#define SETPRECHARGE 		0xD9
//#define SETMULTIPLEX 		0xA8
//#define SETLOWCOLUMN 		0x00
//#define SETHIGHCOLUMN 		0x10
//#define SETSTARTLINE 		0x40
//#define MEMORYMODE 			0x20
//#define COMSCANINC 			0xC0
//#define COMSCANDEC 			0xC8
//#define SEGREMAP 			0xA0
//#define CHARGEPUMP 			0x8D
//#define EXTERNALVCC 		0x01
//#define SWITCHCAPVCC 		0x02
//#define ALL					1
//#define LCDWIDTH			64
//#define LCDHEIGHT			48
//
//// --- OLED control pins (on GPIOA) ---
//#define OLED_RES_LOW()    (GPIOA->ODR &= ~(1 << 0))
//#define OLED_RES_HIGH()   (GPIOA->ODR |= (1 << 0))
//#define OLED_DC_LOW()     (GPIOA->ODR &= ~(1 << 1))
//#define OLED_DC_HIGH()    (GPIOA->ODR |= (1 << 1))
//#define OLED_CS_LOW()     (GPIOA->ODR &= ~(1 << 4))
//#define OLED_CS_HIGH()    (GPIOA->ODR |= (1 << 4))
//
//
//static uint8_t screenmemory [] = {
//	/* LCD Memory organised in 64 horizontal pixel and 6 rows of byte
//
//	*/
//	//SparkFun Electronics LOGO
//
//	// ROW0, BYTE0 to BYTE63
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0x0F, 0x07, 0x07, 0x06, 0x06, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW1, BYTE64 to BYTE127
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x07, 0x0F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xE0,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW2, BYTE128 to BYTE191
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
//	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFD, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW3, BYTE192 to BYTE255
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x07, 0x01,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW4, BYTE256 to BYTE319
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F,
//	0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW5, BYTE320 to BYTE383
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
//	0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//};
//
//// --- Simple software delay ---
//void delay(volatile uint32_t t) {
//    while (t--);
//}

//// --- GPIO setup ---
//void gpio_init(void) {
//    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//
//    // PA0 = RES, PA1 = DC, PA4 = CS => Output
//    GPIOA->MODER &= ~((3 << (0 * 2)) | (3 << (1 * 2)) | (3 << (4 * 2)));
//    GPIOA->MODER |=  ((1 << (0 * 2)) | (1 << (1 * 2)) | (1 << (4 * 2)));
//
//    // PA5 = SCK, PA7 = MOSI => Alternate Function 0 (SPI1)
//    GPIOA->MODER &= ~((3 << (5 * 2)) | (3 << (7 * 2)));
//    GPIOA->MODER |=  ((2 << (5 * 2)) | (2 << (7 * 2)));
//
//    GPIOA->AFR[0] &= ~((0xF << (5 * 4)) | (0xF << (7 * 4))); // Clear AF
//    GPIOA->AFR[0] |=  ((0x0 << (5 * 4)) | (0x0 << (7 * 4))); // AF0 for SPI1
//}
//
//// --- SPI setup ---
//void spi1_init(void) {
//    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//
//    SPI1->CR1 = 0;
//    SPI1->CR1 |= SPI_CR1_MSTR;       // Master mode
//    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Software NSS
//    SPI1->CR1 |= (7 << 3);  // BR[2:0] = 111 => Baud rate = f_PCLK / 256
//    SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // Ensure MSB first
//    SPI1->CR1 |= SPI_CR1_SPE;        // SPI enable
//}
//
//// --- SPI send ---
//void spi1_write(uint8_t data) {
//    while (!(SPI1->SR & SPI_SR_TXE));
//    SPI1->DR = data;
//    while (SPI1->SR & SPI_SR_BSY);
//}
//
//// --- OLED SPI protocol ---
//void oled_send_cmd(uint8_t cmd) {
//    OLED_DC_LOW();
//    OLED_CS_LOW();
//    spi1_write(cmd);
//    OLED_CS_HIGH();
//    delay(1000); // Small delay between commands
//}
//
//void oled_send_data(uint8_t data) {
//    OLED_DC_HIGH();
//    OLED_CS_LOW();
//    spi1_write(data);
//    OLED_CS_HIGH();
//}
//
//// --- OLED Reset ---
//void oled_reset(void) {
//    OLED_RES_LOW();
//    delay(100000);
//    OLED_RES_HIGH();
//    delay(100000);
//}
//
//// --- SSD1306 OLED Initialization ---
//void oled_init(void) {
//
//	oled_reset();
//
//	// Display Init sequence for 64x48 OLED module
//	oled_send_cmd(DISPLAYOFF);			// 0xAE
//
//	oled_send_cmd(SETDISPLAYCLOCKDIV);	// 0xD5
//	oled_send_cmd(0x80);					// the suggested ratio 0x80
//
//	oled_send_cmd(SETMULTIPLEX);			// 0xA8
//	oled_send_cmd(0x2F);
//
//	oled_send_cmd(SETDISPLAYOFFSET);		// 0xD3
//	oled_send_cmd(0x0);					// no offset
//
//	oled_send_cmd(SETSTARTLINE | 0x0);	// line #0
//
//	oled_send_cmd(CHARGEPUMP);			// enable charge pump
//	oled_send_cmd(0x14);
//
//	oled_send_cmd(NORMALDISPLAY);			// 0xA6
//	oled_send_cmd(DISPLAYALLONRESUME);	// 0xA4
//
//	oled_send_cmd(SEGREMAP | 0x1);
//	oled_send_cmd(COMSCANDEC);
//
//	oled_send_cmd(SETCOMPINS);			// 0xDA
//	oled_send_cmd(0x12);
//
//	oled_send_cmd(SETCONTRAST);			// 0x81
//	oled_send_cmd(0x8F);
//
//	oled_send_cmd(SETPRECHARGE);			// 0xd9
//	oled_send_cmd(0xF1);
//
//	oled_send_cmd(SETVCOMDESELECT);			// 0xDB
//	oled_send_cmd(0x40);
//
//	oled_send_cmd(DISPLAYON);				//--turn on oled panel
////	clear(ALL);						// Erase hardware memory inside the OLED controller to avoid random data in memory.
//	printf("Completed Init OLED\r\n");
//
//
//
////    oled_reset();
////
////    oled_send_cmd(0xAE); // Display off
////    oled_send_cmd(0xD5); oled_send_cmd(0x80); // Clock divide
////    oled_send_cmd(0xA8); oled_send_cmd(0x3F); // Multiplex ratio
////    oled_send_cmd(0xD3); oled_send_cmd(0x00); // Display offset
////    oled_send_cmd(0x40); // Start line = 0
////    oled_send_cmd(0x8D); oled_send_cmd(0x14); // Charge pump on
////    oled_send_cmd(0x20); oled_send_cmd(0x00); // Memory mode: horizontal
////    oled_send_cmd(0xA1); // Segment remap
////    oled_send_cmd(0xC8); // COM scan direction
////    oled_send_cmd(0xDA); oled_send_cmd(0x12); // COM pins
////    oled_send_cmd(0x81); oled_send_cmd(0xCF); // Contrast
////    oled_send_cmd(0xD9); oled_send_cmd(0xF1); // Pre-charge
////    oled_send_cmd(0xDB); oled_send_cmd(0x40); // VCOM detect
////    oled_send_cmd(0xA4); // Resume display RAM
////    oled_send_cmd(0xA6); // Normal display
////    oled_send_cmd(0xAF); // Display ON
//}
///** \brief Set SSD1306 page address.
//
//    Send page address command and address to the SSD1306 OLED controller.
//*/
//void setPageAddress(uint8_t add) {
//	add=0xb0|add;
//	oled_send_cmd(add);
//	return;
//}
//
///** \brief Set SSD1306 column address.
//
//    Send column address command and address to the SSD1306 OLED controller.
//*/
//void setColumnAddress(uint8_t add) {
//	oled_send_cmd((0x10|(add>>4))+0x02);
//	oled_send_cmd((0x0f&add));
//	return;
//}
//
//
//
//void display(void) {
//	printf("Display\r\n");
//	uint8_t i, j;
//
//	for (i=0; i<6; i++) {
//		setPageAddress(i);
//		setColumnAddress(0);
//		for (j=0;j<0x40;j++) {
//			oled_send_data(screenmemory[i*0x40+j]);
//		}
//	}
//}
//
//// --- Fill display with pattern ---
//void oled_test_pattern(void) {
//    for (uint8_t page = 0; page < 8; page++) {
//        oled_send_cmd(0xB0 + page); // Set page address
//        oled_send_cmd(0x00);        // Set lower column
//        oled_send_cmd(0x10);        // Set upper column
//
//        for (uint8_t col = 0; col < 128; col++) {
//            oled_send_data(0xFF); // All pixels ON
//        }
//    }
//}
//
//// --- Main ---
//int main(void) {
//    gpio_init();
//    spi1_init();
//    oled_init();
//
//    while(1){
//    	display();
//    	delay(1000000);
//    }
//
//   // oled_test_pattern(); // Fill screen
//
//    while (1) {
//        // Idle loop
//    }
//}





///**
// ******************************************************************************
// * @file           : main.c
// * @author         : Jasey Chanders
// * @brief          : Transitions a status LED for an airport train through three stations:
// * 				   : T (terminal), A (Concourse), S (Storage). LEDs blink when
// * 				   : in a station and fade between station brightness levels
// * 				   : when traveling. At any point an emergency button can be pressed
// * 				   : which 'pauses' the train wherever it is and blinks the LED until
// * 				   : the button is released and the train returns to normal operation from
// * 				   : wherever it was paused.
// ******************************************************************************
// * @attention
// *
// * Copyright (c) 2023 STMicroelectronics.
// * All rights reserved.
// *
// * This software is licensed under terms that can be found in the LICENSE file
// * in the root directory of this software component.
// * If no LICENSE file comes with this software, it is provided AS-IS.
// *
// ******************************************************************************
// */
//
#include <stdio.h>
#include "stm32f0xx.h"
//#include "utilities.h"
//#include <stdbool.h>
//#include "log.h"
//#include "led.h"
//#include "switch.h"
//
////Conversion from msec to iterations based on stop watch timing
//#define MS_TO_ITERATIONS 3195
//
//#define DC_LOW_MSK (GPIO_BSRR_BR_9)
//#define DC_HIGH_MSK (GPIO_BSRR_BS_9)
//
//#define SS_LOW_MSK (GPIO_BSRR_BR_6)
//#define SS_HIGH_MSK (GPIO_BSRR_BS_6)
//
//#define RST_LOW_MSK (GPIO_BSRR_BR_7)
//#define RST_HIGH_MSK (GPIO_BSRR_BS_7)
//
//#define SETCONTRAST 		0x81
//#define DISPLAYALLONRESUME 	0xA4
//#define DISPLAYALLON 		0xA5
//#define NORMALDISPLAY 		0xA6
//#define INVERTDISPLAY 		0xA7
//#define DISPLAYOFF 			0xAE
//#define DISPLAYON 			0xAF
//#define SETDISPLAYOFFSET 	0xD3
//#define SETCOMPINS 			0xDA
//#define SETVCOMDESELECT		0xDB
//#define SETDISPLAYCLOCKDIV 	0xD5
//#define SETPRECHARGE 		0xD9
//#define SETMULTIPLEX 		0xA8
//#define SETLOWCOLUMN 		0x00
//#define SETHIGHCOLUMN 		0x10
//#define SETSTARTLINE 		0x40
//#define MEMORYMODE 			0x20
//#define COMSCANINC 			0xC0
//#define COMSCANDEC 			0xC8
//#define SEGREMAP 			0xA0
//#define CHARGEPUMP 			0x8D
//#define EXTERNALVCC 		0x01
//#define SWITCHCAPVCC 		0x02
//#define ALL					1
//#define LCDWIDTH			64
//#define LCDHEIGHT			48
//
//
//
///*
// * Pinout
// *
//      MicroOLED ------------- STM32F091RC
//      GND ------------------- GND
//      VDD ------------------- 3.3V (VCC)
//    D1/MOSI ----------------- D11
//    D0/SCK ------------------ D13 (don't change)
//      D2
//      D/C ------------------- D8 (can be any digital pin)
//      RST ------------------- D9 (can be any digital pin)
//      CS  ------------------- D10 (can be any digital pin)
// */
//
///** \brief MicroOLED screen buffer.
//
//Page buffer 64 x 48 divided by 8 = 384 bytes
//Page buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.  This page buffer serves as a scratch RAM for graphical functions.  All drawing function will first be drawn on this page buffer, only upon calling display() function will transfer the page buffer to the actual LCD controller's memory.
//*/
//static uint8_t screenmemory [] = {
//	/* LCD Memory organised in 64 horizontal pixel and 6 rows of byte

//	*/
//	//SparkFun Electronics LOGO
//
//	// ROW0, BYTE0 to BYTE63
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0x0F, 0x07, 0x07, 0x06, 0x06, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW1, BYTE64 to BYTE127
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x07, 0x0F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xE0,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW2, BYTE128 to BYTE191
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
//	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFD, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW3, BYTE192 to BYTE255
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x07, 0x01,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW4, BYTE256 to BYTE319
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F,
//	0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//
//	// ROW5, BYTE320 to BYTE383
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
//	0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//};
//
//uint8_t bender [] = {
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF, 0x5F, 0x5F, 0x5F, 0x5F,
//0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
//0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
//0x5F, 0xDF, 0xBF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xF9, 0xFE, 0x07, 0x01, 0x00, 0x00, 0xF8, 0xFE, 0xFF,
//0xFF, 0xFF, 0x1F, 0x1F, 0x1F, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00,
//0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0x1F, 0x1F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8,
//0x00, 0x00, 0x01, 0x07, 0xFE, 0xF9, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF9, 0xE7, 0xDC, 0xB0, 0xA0, 0x40, 0x41, 0x47, 0x4F,
//0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x4F, 0x47, 0x43, 0x40, 0x40, 0x40, 0x40,
//0x43, 0x47, 0x4F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x4F, 0x47, 0x43, 0x40,
//0x40, 0xA0, 0xB0, 0xDE, 0xE7, 0xF9, 0xFE, 0x1F, 0x0F, 0x07, 0x73, 0x79, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
//0xBF, 0x5F, 0xEF, 0x0F, 0xEF, 0xEF, 0xDF, 0xDF, 0x1F, 0xDF, 0xDF, 0xDF, 0xDF, 0x1F, 0xDF, 0xDF,
//0xDF, 0xDF, 0xDF, 0x1F, 0xDF, 0xDF, 0xDF, 0xEF, 0x0F, 0xEF, 0xDF, 0xBF, 0x7F, 0xFF, 0xFF, 0xFF,
//0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xBE, 0x9C, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
//0xB7, 0x6F, 0xEE, 0x00, 0xDE, 0xDE, 0xDE, 0xDD, 0x00, 0xDD, 0xDD, 0xDD, 0xDD, 0x00, 0xDD, 0xDD,
//0xDD, 0xC5, 0xC1, 0x00, 0xC9, 0xC5, 0xC1, 0x01, 0xC8, 0xC4, 0x42, 0x80, 0xC0, 0xE8, 0xE4, 0xE2,
//0xE0, 0xE0, 0xEF, 0xEF, 0xE6, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFE, 0xFE, 0xFD, 0xFD, 0xFD, 0xFB, 0xF8, 0xFB, 0xFB, 0xFB, 0xFB, 0xF8, 0xFB, 0xFB,
//0xFB, 0xFB, 0xFB, 0xF8, 0xFB, 0xFD, 0xFD, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
//};
//
///*
//Draw Bitmap image on screen. The array for the bitmap can be stored in the Arduino file, so user don't have to mess with the library files.
//To use, create uint8_t array that is 64x48 pixels (384 bytes). Then call .drawBitmap and pass it the array.
//*/
//void drawBitmap(uint8_t * bitArray)
//{
//  for (int i=0; i<(LCDWIDTH * LCDHEIGHT / 8); i++)
//    screenmemory[i] = bitArray[i];
//}
//
//// Blocking delay
//void delay(int msec){
//
////	LOG("START TIMER %i for starting a %i msec delay\r\n", msec, msec);
//
//	int iterations = msec * MS_TO_ITERATIONS;
//	while (iterations-- != 0) { //spins desired num clock cycles
//		__asm volatile("NOP");
//	};
//}
//
//
//void SPI_Send_Byte(uint8_t d_out) {
//	//LOG("Sending byte %i\r\n", d_out);
//	// Wait until transmitter buffer is empty
//	while ((SPI1->SR & SPI_SR_TXE) == 0)
//		 ;
//	// Transmit d_ out
//	// Must tell compiler to use a byte write (not half- word)
//	// by casting SPI1- >DR into a pointer to a byte (uint8_ t).
//	// See STM32F0 Snippets (SPI_ 01_ FullDuplexCommunications).
//	*((uint8_t *)&(SPI1->DR)) = d_out;
//};
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
//	LOG("Received %i\r\n", d_in);
//	return d_in;
//};
//
//void command(uint8_t c) {
////	LOG("Send command %i\r\n", c);
//
//	GPIOA->BSRR |= DC_LOW_MSK; // DC pin LOW for a command
//	GPIOB->BSRR |= SS_LOW_MSK;	// SS LOW to initialize transfer
//	SPI_Send_Byte(c);			// Transfer the command byte
//	GPIOB->BSRR |= SS_HIGH_MSK;	// SS HIGH to end transfer
//}
//
//void reset_display(){
////	LOG("Reset Display\r\n");
//	// Display reset routine
//	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER7, ESF_GPIO_MODER_OUTPUT);	// Set RST pin as OUTPUT
//	GPIOA->BSRR |= RST_HIGH_MSK;	// Initially set RST HIGH
//	delay(5);	// VDD (3.3V) goes high at start, lets just chill for 5 ms
//	GPIOA->BSRR |= RST_LOW_MSK;	// Bring RST low, reset the display
//	delay(10);	// wait 10ms
//	GPIOA->BSRR |= RST_HIGH_MSK;	// Set RST HIGH, bring out of reset
//
//}
//
///** \brief Clear screen buffer or SSD1306's memory.
//
//    To clear GDRAM inside the LCD controller, pass in the variable mode = ALL and to clear screen page buffer pass in the variable mode = PAGE.
//*/
////void clear(uint8_t mode) {
////	//	uint8_t page=6, col=0x40;
////	if (mode==ALL) {
////		for (int i=0;i<8; i++) {
////			setPageAddress(i);
////			setColumnAddress(0);
////			for (int j=0; j<0x80; j++) {
////				data(0);
////			}
////		}
////	}
////	else
////	{
////		memset(screenmemory,0,384);			// (64 x 48) / 8 = 384
////		//display();
////	}
////}
//

//
///** \brief Transfer display memory.
//
//    Bulk move the screen buffer to the SSD1306 controller's memory so that images/graphics drawn on the screen buffer will be displayed on the OLED.
//*/
//void display(void) {
////	LOG("Display\r\n");
//	uint8_t i, j;
//
//	for (i=0; i<6; i++) {
//		setPageAddress(i);
//		setColumnAddress(0);
//		for (j=0;j<0x40;j++) {
//			data(screenmemory[i*0x40+j]);
//		}
//	}
//}
//
//void Init_SPI1(void) {
//	// Clock gating for SPI1 and GPIO A and B
//	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
//
//	//Chip Select
//	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER6, ESF_GPIO_MODER_OUTPUT);
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
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 0);
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 0);
//	// Data is LSB first
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 0);
//	// Data is 8 bits long
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_DS, 7);
//	// RXNE when at least 1 byte in RX FIFO
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_FRXTH, 1);
//	// Have NSS pin asserted automatically
//	MODIFY_FIELD(SPI1->CR2, SPI_CR2_NSSP, 1);
//	// Enable SPI
//	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);
//
//
//
//}
//
//void init_OLED(){
//	LOG("Init OLED\r\n");
//	//DC
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER9,
//	ESF_GPIO_MODER_OUTPUT);
//
//	//RST
//	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER7,
//	ESF_GPIO_MODER_OUTPUT);
//
//	Init_SPI1();
//	reset_display();
//
//	// Display Init sequence for 64x48 OLED module
//	command(DISPLAYOFF);			// 0xAE
//
//	command(SETDISPLAYCLOCKDIV);	// 0xD5
//	command(0x80);					// the suggested ratio 0x80
//
//	command(SETMULTIPLEX);			// 0xA8
//	command(0x2F);
//
//	command(SETDISPLAYOFFSET);		// 0xD3
//	command(0x0);					// no offset
//
//	command(SETSTARTLINE | 0x0);	// line #0
//
//	command(CHARGEPUMP);			// enable charge pump
//	command(0x14);
//
//	command(NORMALDISPLAY);			// 0xA6
//	command(DISPLAYALLONRESUME);	// 0xA4
//
//	command(SEGREMAP | 0x1);
//	command(COMSCANDEC);
//
//	command(SETCOMPINS);			// 0xDA
//	command(0x12);
//
//	command(SETCONTRAST);			// 0x81
//	command(0x8F);
//
//	command(SETPRECHARGE);			// 0xd9
//	command(0xF1);
//
//	command(SETVCOMDESELECT);			// 0xDB
//	command(0x40);
//
//	command(DISPLAYON);				//--turn on oled panel
////	clear(ALL);						// Erase hardware memory inside the OLED controller to avoid random data in memory.
//	LOG("Completed Init OLED\r\n");
//}
//
//
//
//
//void Test_SPI_Loopback(void) {
//	uint8_t out = 'A';
//	uint8_t in;
//	while (1) {
//		in = SPI_Send_Receive_Byte(out);
//		// printf("out = %i, in = %i\r\n", out, in);
//
//		 if (in != out){ // Red: error, data does not match
//			 set_led(ELED, ON);
//		 }else{			 // Green: data matches
//			 set_led(ULED, ON);
//		 }
//			 out++;
//		 if (out > 'z')
//			 out = 'A';
//	}
//}
//
///*
// * @breif   : User switch triggered interrupt
// * 			: Triggers on rising and falling edge of button
// * 			: Code modified from Dean 4.8, 4.9 4.12
// * @param   : void
// * @return  : void
// */
////void EXTI4_15_IRQHandler(void) {
////	//Get g_state of enable/disable for interrupts
////	uint32_t masking_g_state = __get_PRIMASK();
////	__disable_irq();
////	//Check for rising edge
////	if ((EXTI->PR & SWITCH_PIN_MASK) != 0) {
////		EXTI->PR = SWITCH_PIN_MASK; // clear pending request
////		if (get_switch_state()) {
////			set_led(ELED, ON);
////			printf("button pressed\r\n");
////
////		} else { //Falling edge
////			set_led(ELED, OFF);
////			printf("button released\r\n");
////		}
////	}
////	// Clear all other pending requests for this handler
////	EXTI->PR = 0x0000fff0;
////	//Return to previous g_state of enable/disable for interrupts
////	__set_PRIMASK(masking_g_state);
////}
//
//
//

/*
 * @Brief   : Main loop
 */
//int main(void)
//{
//	LOG("Main Loop Starting\r\n");
////
////	init_led();
////	///Init_SPI1();
////	init_OLED();
////	//set_led(ELED, ON);
////	while(1){
////		display();
////	}
//	gpio_init();
//	spi1_init();
//	oled_init();
//
//	// Example: Fill display with pattern
//	for (uint8_t page = 0; page < 8; page++) {
//		oled_send_command(0xB0 + page);       // Set page address
//		oled_send_command(0x00);              // Set lower column address
//		oled_send_command(0x10);              // Set higher column address
//
//		for (uint8_t col = 0; col < 128; col++) {
//			oled_send_data(0xFF); // All pixels on in this column
//		}
//	}
//
//	while (1) {
//		// Infinite loop, you could toggle a GPIO here for testing
//	}
//	//drawBitmap(bender);//call the drawBitmap function and pass it the array from above
//    //Test_SPI_Loopback();
//	//init_switch();
//
//	for(;;);
//
//}
