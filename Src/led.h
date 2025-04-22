/**
 ******************************************************************************
 * @file           : led.h
 * @author         : Jasey Chanders
 * @brief          : Allows for simple access to User LED and an external LED
 * @date           : Mar 1, 2025
 ******************************************************************************
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f0xx.h"
#include "log.h"
#include "utilities.h"


//on/off for readability
#define ON 1
#define OFF 0

#define ULED 0
#define ELED 1

/*
 * @breif Initializes the LEDS
 *
 * @return void
 */
void init_led();

/*
 * @breif sets a desired LED to a desired state
 *
 * @param state  : the state to set the LED to. 1 = on, 0 = off
 *
 * @return void
 */
void set_led(int id, int state);

#endif
