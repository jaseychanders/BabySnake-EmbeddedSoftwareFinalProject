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

// User switch on PORT C, pin 13
#define SWITCH_PIN_MASK GPIO_IDR_13


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


#endif
