#include "stm32f0xx.h"
#include "utilities.h"

#define SS_LOW_MSK (GPIO_BSRR_BR_15)
#define SS_HIGH_MSK (GPIO_BSRR_BS_15)


void OLED_SPI_Pins_Init()
{
	RCC->AHBENR|=RCC_AHBENR_GPIOAEN; //enable clock for GPIOA
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN; //enable clock for GPIOA

	//set PA5, PA6 and PA7 to alternate function mode
//	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1|GPIO_MODER_MODE7_1;
//	GPIOA->MODER &=~(GPIO_MODER_MODE5_0|GPIO_MODER_MODE6_0|GPIO_MODER_MODE7_0);

	// GPIO B pin 3, 4, 5 in alternate function 0 (SPI1) for SCK, MISO, MOSI
	// Set each mode field to 2 for alternate function
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, 2);
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER4, 2);
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER5, 2);
	// Select SPI1 (AF = 0) for alternate function
	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL3, 0);
	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL4, 0);
	MODIFY_FIELD(GPIOB->AFR[0], GPIO_AFRL_AFSEL5, 0);

	//Chip Select
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER15, ESF_GPIO_MODER_OUTPUT);
	//MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRH_AFSEL15, 0);



//	//Set PA9 and PA10 as Output
//	GPIOA->MODER|=GPIO_MODER_MODE9_0|GPIO_MODER_MODE10_0;
//	GPIOA->MODER&=~(GPIO_MODER_MODE9_1|GPIO_MODER_MODE10_1);

	/*select which AF for PA5, PA6 and PA7*/
	//GPIOA->AFR[0]|=(0x05<<20)|(0x05<<24)|(0x05<<28);
}

void OLED_SPI_Configure()
{
	/*Enable clock access to SPI1 module*/
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

		/*Set clock to fPCLK/2*/
		SPI1->CR1 &=~(1U<<3);
		SPI1->CR1 &=~(1U<<4);
		SPI1->CR1 &=~(1U<<5);

		/*Enable full duplex*/
		SPI1->CR1 &=~(1U<<10);

		/*Set MSB first*/
		SPI1->CR1 &= ~(1U<<7);

		// Select first edge sample, active high clock
		MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 1);
		MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 1);

		/*Set mode to MASTER*/
		SPI1->CR1 |= (1U<<2);

		/*Set 8 bit data mode*/
		SPI1->CR1 &= ~(1U<<11);

		/*Select software slave management by
		 * setting SSM=1 and SSI=1*/
		SPI1->CR1 |= (1<<8);
		SPI1->CR1 |= (1<<9);

		/*Enable SPI module*/
		SPI1->CR1 |= (1<<6);
}

void OLED_SPI_Write(char *data,uint32_t size)
{
	GPIOA->BSRR |= SS_LOW_MSK;
	uint32_t i=0;

	while(i<size)
	{
		/*Wait until TXE is set*/
		while(!(SPI1->SR & (SPI_SR_TXE))){}

		/*Write the data to the data register*/
		SPI1->DR =(uint8_t) data[i];
		i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI1->SR & (SPI_SR_TXE))){}

	/*Wait for BUSY flag to reset*/
	while((SPI1->SR & (SPI_SR_BSY))){}

	/*Clear OVR flag*/
	(void)SPI1->DR;
	(void)SPI1->SR;

	GPIOA->BSRR |= SS_HIGH_MSK;
}

//void OLED_Select(void)
//{
//	GPIOA->BSRR =GPIO_BSRR_BR9;
//
//}
//
///*Pull high to disable*/
//void OLED_Deselect(void)
//{
//	GPIOA->BSRR =GPIO_BSRR_BS9;
//}
//
//void OLED_DataMode()
//{
//	GPIOA->BSRR=GPIO_BSRR_BS10;
//}
//
//void OLED_CommMode()
//{
//	GPIOA->BSRR=GPIO_BSRR_BR10;
//}
//
//void SSD1306_WRITEDATA(char command)
//{
//	OLED_DataMode();
//	OLED_Select();
//	OLED_SPI_Write(&command,1);
//	OLED_Deselect();
//}
//
//void SSD1306_WRITECOMMAND(char command)
//{
//	OLED_CommMode();
//	OLED_Select();
//	OLED_SPI_Write(&command,1);
//	OLED_Deselect();
//}
//
//void SSD1306_Write_Multi_Data(char * data, uint16_t length)
//{
//	OLED_DataMode();
//	OLED_Select();
//	OLED_SPI_Write((char*)data,length);
//	OLED_Deselect();
//
//}
//
//uint8_t SSD1306_Init(void)
//{
//	 OLED_SPI_Pins_Init();
//
//	 OLED_SPI_Configure();
//	/* A little delay */
//	uint32_t p = 2500;
//	while(p>0)
//		p--;
//
//	/* Init LCD */
//	SSD1306_WRITECOMMAND(0xAE); //display off
//	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode
//	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
//	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
//	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
//	SSD1306_WRITECOMMAND(0x00); //---set low column address
//	SSD1306_WRITECOMMAND(0x10); //---set high column address
//	SSD1306_WRITECOMMAND(0x40); //--set start line address
//	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
//	SSD1306_WRITECOMMAND(0xFF);
//	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
//	SSD1306_WRITECOMMAND(0xA6); //--set normal display
//	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
//	SSD1306_WRITECOMMAND(0x3F); //
//	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
//	SSD1306_WRITECOMMAND(0xD3); //-set display offset
//	SSD1306_WRITECOMMAND(0x00); //-not offset
//	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
//	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
//	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
//	SSD1306_WRITECOMMAND(0x22); //
//	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
//	SSD1306_WRITECOMMAND(0x12);
//	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
//	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
//	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
//	SSD1306_WRITECOMMAND(0x14); //
//	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
//
//
//	SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);
//
//	/* Clear screen */
//	SSD1306_Fill(SSD1306_COLOR_BLACK);
//
//	/* Update screen */
//	SSD1306_UpdateScreen();
//
//	/* Set default values */
//	SSD1306.CurrentX = 0;
//	SSD1306.CurrentY = 0;
//
//	/* Initialized OK */
//	SSD1306.Initialized = 1;
//
//	/* Return OK */
//	return 1;
//}
//
//void SSD1306_GotoXY(uint16_t x, uint16_t y) {
//	/* Set write pointers */
//	SSD1306.CurrentX = x;
//	SSD1306.CurrentY = y;
//}
//
//char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
//	/* Write characters */
//	while (*str) {
//		/* Write character by character */
//		if (SSD1306_Putc(*str, Font, color) != *str) {
//			/* Return error */
//			return *str;
//		}
//
//		/* Increase string pointer */
//		str++;
//	}
//
//	/* Everything OK, zero should be returned */
//	return *str;
//}
//
//void SSD1306_UpdateScreen(void) {
//	uint8_t m;
//
//	for (m = 0; m < 8; m++) {
//		SSD1306_WRITECOMMAND(0xB0 + m);
//		SSD1306_WRITECOMMAND(0x00);
//		SSD1306_WRITECOMMAND(0x10);
//
//		/* Write multi data */
//
//		SSD1306_Write_Multi_Data(&SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
//	}
//}

//int main(void){
//	OLED_SPI_Pins_Init();
//	OLED_SPI_Configure();
//
//	char a = 0xaa;
//	while(1)
//	{
//		OLED_SPI_Write(&a, 1);
//	}
//}

