/**
 ******************************************************************************
 * @file           : switch.h
 * @author         : Jasey Chanders
 * @brief          : Allows for simple access to User switch
 * @date           : Mar 1, 2025
 ******************************************************************************
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "stm32f0xx.h"
#include "utilities.h"
#include <stdbool.h>
#include "log.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

/*
 * @breif Initializes the user switch
 *
 * @return void
 */
void init_switch();

/*
 * @breif gets the state of the switch
 *
 * @return true if switch is pressed, false otherwise
 */
bool get_switch_state();


void clear_flags();

uint8_t get_flags();


#endif
