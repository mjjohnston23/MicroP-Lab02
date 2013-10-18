#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "mode.h"
#include "handleSysTick.h"
#include "pulse.h"
#include "leds.h"

enum mode activeMode;

/**
 * initializes the GPIO for the user button
*/
void initializeButton(){
	  GPIO_InitTypeDef gpio_init_s;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
        
    GPIO_StructInit(&gpio_init_s);
    gpio_init_s.GPIO_Pin = GPIO_Pin_0;
    gpio_init_s.GPIO_Mode = GPIO_Mode_IN;
    gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_s.GPIO_OType = GPIO_OType_PP;
    gpio_init_s.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &gpio_init_s);
}

/**
 * initializes the system in temp sensing mode
*/
void initializeMode(){
		activeMode = temp;
}
/**
 * checks to see if the button is pushed
*/
void buttonPushed(){
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
		toggleMode();
		debounce();
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){ //if it takes more than 1 debounce cycle for button to reset
			debounce();
		}
	}
}
/**
 * switches the mode from temp sensing to led pulsing, or vise versa depending upon the active state
 * also adjusts the frequency of SysTicks timer
*/
void toggleMode(){
	if (activeMode == temp){
		activeMode = pulse;
		acquiringFreq = 10000;
		uint32_t result = SysTick_Config(SystemCoreClock / acquiringFreq);

		startPulse();
	}
	else{
		activeMode = temp;
		acquiringFreq = 20;
		uint32_t result = SysTick_Config(SystemCoreClock / acquiringFreq);
		startLeds();
	}
}

/**
 * provides a 250 ms debounce period
*/
void debounce(){
	uint32_t timer = 0;
	while (timer < 1000){
		timer++;
	}
	printf("exited while loop");
}

