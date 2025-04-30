/**
 ******************************************************************************
 * @file           : switch.h
 * @author         : Jasey Chanders
 * @brief          : Reads 4 external switches using interrupts and updates flags
 * @date           : April 30th, 2025
 ******************************************************************************
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "stm32f0xx.h"
#include "utilities.h"
#include <stdbool.h>
#include "log.h"

#define UP 					0
#define RIGHT 				1
#define DOWN 				2
#define LEFT 				3

#define UP_SWITCH_MSK		0x01
#define RIGHT_SWITCH_MSK	0x02
#define DOWN_SWITCH_MSK		0x04
#define LEFT_SWITCH_MSK		0x08

/*
 * @Breif		: Initializes the switches and interrupts
 *
 * @References	: Embedded Systems Fundamentals by Alexander Dean Listings 4.8, 4.9 4.12
 */
void init_switches();

/*
 * @Breif		: Clears the flags
 */
void clear_flags();

/*
 * @Breif		: Gets the current flags
 *
 * @Return		: The current flags
 * 				: Each switch is represented by a single bit
 * 				: LSB => 		Up switch
 * 				: LSB + 1 => 	Right switch
 * 				: LSB + 2 => 	Down switch
 * 				: LSB + 3 => 	Left switch
 */
uint8_t get_flags();


#endif
