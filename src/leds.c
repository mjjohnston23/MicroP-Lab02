#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "gpio_example.h"

/**
 * Variables needed for cool LED action
*/

enum led {top, right, bottom, left};
enum led activeLed;

/**
 * starts the LEDs with the top one activated
*/
void startLeds(){
	activeLed = top;
	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	printf("lights on");
}

/**
 * rotates the active LED in a clockwise manner (increase in temp)
*/
void cwLeds(enum led activeLed){
	
}
