/**
 * Main class of ECSE-426 Lab02.
*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "gpio_example.h"
#include "adc.h"
#include "leds.h"
#include "tempFilter.h"
#include "tempStatus.h"
#include "handleSysTick.h"






/**
 * Defines functions used in the main file.
*/
void mainPreamble();


/**
 * Main method for Lab03.
*/
int main() {
	mainPreamble();
	
	int counter = 0;
	while (1) {
		// Here we wait for the interrupt to happen.
		while (!tick);
		//printf("tick %d ", counter);
		counter++;
		resetTick();
		acquireADCValue();
	
		
	}
	
	return (0);
}



/**
 * Sets all the pre-conditions for the main method to execute properly.
*/
void mainPreamble() {
	// Sets tick to 0.
	resetTick();
	
	// Initialize the clocks.
	SystemInit();
	
	//Get the LEDs running
	startLeds();
	
		
	// Sets the SysTick delay needed to sample the ADC at the required rate.
	uint32_t result = SysTick_Config(SystemCoreClock / acquiringFreq);
		
	// Handles the error if something went wrong.
	if (result != 0) {
		printf("An error occured when setting the SYSTICK timer. Terminating the program now.");
		exit(EXIT_FAILURE);
	}
	
	configureADC();
}




	

