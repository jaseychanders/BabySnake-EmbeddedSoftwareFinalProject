#include "delay.h"

// Blocking delay
void delay_ms(int msec){

//	LOG("START TIMER %i for starting a %i msec delay\r\n", msec, msec);

	int iterations = msec * MS_TO_ITERATIONS;
	while (iterations-- != 0) { //spins desired num clock cycles
		__asm volatile("NOP");
	}
}
