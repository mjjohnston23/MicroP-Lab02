/**
 * Authors: M. Johnston and J.S. Dery
 * Handles led-related behaviour in temp sensing mode
*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "gpio_example.h"
#include "leds.h"
#include "tempStatus.h"
#include "mode.h"
#include "pulse.h"

/**
 * Variables needed for cool LED action
*/

enum led {top, right, bottom, left};
enum led activeLed;

/**
 * starts the LEDs with the top one activated
*/
void startLeds(){
	
	 GPIO_InitTypeDef gpio_init_s;
        
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Enable clock to LEDs
        
   GPIO_StructInit(&gpio_init_s);
   gpio_init_s.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
   gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
   gpio_init_s.GPIO_OType = GPIO_OType_PP;
   gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOD, &gpio_init_s);
	
	if (activeMode == temp){
		//temp mode
		activeLed = top;
		switchLed();
	}
	else {
		//we are in pulsing mode
		startPulse();
	}
	
}
/**
 * determine whether we need to rotate the active light depending on temp deviation from previous rotation
*/
void ledState(){
	if (deviation > 1){
		cwLeds();
		deviation = 0;//we have gone +2 deg C, reset deviation
	}
	else if (deviation < -1){
		ccwLeds();
		deviation = 0;//we have gone -2 deg C, reset deviation
	}
	else{
		//do nothing
	}
		
}

/**
 * rotates the active LED in a clockwise manner (increase in temp)
*/
void cwLeds(){
	if (activeLed == top){
		activeLed = right;
	}
	else if (activeLed == right){
		activeLed = bottom;
	}
	else if (activeLed == bottom){
		activeLed = left;
	}
	else{
		activeLed = top;
	}
	switchLed();
}
/**
 * rotates the active LED in a counter-clockwise manner (decrease in temp)
*/
void ccwLeds(){
		if (activeLed == top){
		activeLed = left;
	}
	else if (activeLed == left){
		activeLed = bottom;
	}
	else if (activeLed == bottom){
		activeLed = right;
	}
	else{
		activeLed = top;
	}
	switchLed();
}
/**
 * activates the correct led
*/
void switchLed(){
	GPIO_Write(GPIOD, 0x0); //turn off the leds
	if (activeLed == top){
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	}
	else if (activeLed == right){
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
	}
	else if (activeLed == bottom){
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
	}
	else if (activeLed == left){
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	}
}
	


