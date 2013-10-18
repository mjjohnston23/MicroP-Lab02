#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "handleSysTick.h"
 
uint32_t acquiringFreq = 20;
volatile uint32_t tick = 0;

/**
 * Resets the tick interrupt variable to 0.
*/
void resetTick() {
	tick = 0;
}

/**
 * Interrupt handler for SysTick.
 * When the actual interrupt will happen, this method will be called and its purpose
 * is to set the tick boolean to 1 (so the main method knows that the interrupt happened).
*/
void SysTick_Handler() {
	//printf("ticktock");
//	printf("acqFreq: %d", acquiringFreq);
	tick = 1;
}