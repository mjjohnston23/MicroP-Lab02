/**
 * Authors: M. Johnston and J.S. Dery
 * Handles the pulsing led mode
*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "leds.h"
#include "pulse.h"

uint32_t pulseFreq = 1; // 1 Hz, change to adjust dim/raise periods
uint32_t switchFreq = 10000; // 10kHz
uint32_t dutyCycle; // percent
uint32_t stepSize; //number of switching periods for each Duty Cycle Step
uint32_t tickCounter = 0; //tells us which tick we are on for a given duty cycle

enum direction {raise, dim};
enum direction trend;

/**
 * Turn all the leds on full brightness
*/
void startPulse(){
	dutyCycle = 100;
	trend = dim;	
	stepSize = (1 / pulseFreq)* (switchFreq / (2 * dutyCycle)); // =50 b/c 200 steps round trip
}
/**
 * Called every switching cycle, determine what needs to be changed with the leds
*/
void changeLeds(){
	if (trend == dim && dutyCycle == 0){		
		//we are dimmed to minimum
		trend = raise;
		ledsOff();//redundancy check
	}
	else if(trend == raise && dutyCycle == 100){
		//we are raised to maximum
		trend = dim;
		ledsOn();//redudancy check
	}
	else {
		//we are in between max and min
		if(tickCounter < stepSize){
			//leds stay on
			if (tickCounter > dutyCycle){ 
				//T_on complete, time for T_off
				ledsOff();
			}
			tickCounter++;
		}
		else{//tickCounter == stepSize
			tickCounter = 0;//reset tickCounter
			ledsOn();
			//we change duty cycle by 2% at at time b/c we have 2*50 steps to do a full cycle
			if (trend == dim){
				dutyCycle -= 2;
			}
			else{
				dutyCycle += 2;
			}
			//printf("\ndutyCycle: %d", dutyCycle);
		}
	}
}



/**
 * switch on leds
*/
void ledsOn(){
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);
}
/**
 * switch off leds
*/
void ledsOff(){
	GPIO_Write(GPIOD, 0x0); //turn off the leds
}



