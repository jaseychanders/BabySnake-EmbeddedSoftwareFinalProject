/**
 ******************************************************************************
 * @file           : delay.c
 * @author         : Jasey Chanders
 * @brief          : Simple Blocking Delay
 * @date           : Mar 1, 2025
 ******************************************************************************
 */


#include "delay.h"

//See Header for details
void delay_ms(int msec){

	int iterations = msec * MS_TO_ITERATIONS;
	while (iterations-- != 0) { //spins desired num clock cycles
		__asm volatile("NOP");
	}
}
