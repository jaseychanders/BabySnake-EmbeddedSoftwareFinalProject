/**
 ******************************************************************************
 * @file           : display.h
 * @author         : Jasey Chanders
 * @brief          : Control for a small SSD1306 OLED display. Allows for x,y
 * 				   : setting of a single pixel
 * @References 	   : This heavily based on the SFE_MicroOLED.h arduino library
 * 				   : from SparkFun Electronics as well as Embedded Systems
 * 				   : Fundamentals by ALexander G. Dean. ChatGPT was also consulted
 * @date           : April 27, 2025
 ******************************************************************************
 */


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stm32f0xx.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Screen Dims
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48

/*
 * @Breif : Initializes the display including hardware init and OLED boot up
 * 		  : Clears screen memory and pushes blank display
 */
void init_display(void);

/*@Brief  : Clear screen memory
*/
void clear_display(void);

/*@Brief  : Set screen to fail screen
*/
void fail_screen(void);

/*@Brief  	 : Transfer display memory to OLED
*         	 : Bulk move the screen memory to the SSD1306 controller's memory so that images/graphics drawn on the screen buffer will be displayed on the OLED.
*
*@References : SparkFun Electronics SFE_MicroOLED.h arduino library
*/
void push_display(void);

/*
 * @Breif   : Set the value of a pixel in screen memory
 *
 * @param x : The row [0, screen_height]
 * 		  y : The Column [0, screen_width]
 * 		val : [ON, OFF]
 */
void set_pixel(uint8_t y, uint8_t x, uint8_t value);

#endif
