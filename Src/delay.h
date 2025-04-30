/**
 ******************************************************************************
 * @file           : delay.h
 * @author         : Jasey Chanders
 * @brief          : Simple Blocking Delay
 * @date           : Mar 1, 2025
 ******************************************************************************
 */

#ifndef DELAY_H_
#define DELAY_H_

#define MS_TO_ITERATIONS 3195 // Clock tick to msec conversions

/* @Breif		: Blocking delay
 *
 * @param msec	: Number of miliseconds to delay for
 */
void delay_ms(int msec);

#endif
