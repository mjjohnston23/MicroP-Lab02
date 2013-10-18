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
void changeLeds(){
	if (trend == dim && dutyCycle == 0){		
		trend = raise;
		ledsOff();//redundancy check
	}
	else if(trend == raise && dutyCycle == 100){
		trend = dim;
		ledsOn();//redudancy check
	}
	else {
		if(tickCounter < stepSize){
			if (tickCounter > dutyCycle){ 
				ledsOff();
			}
			tickCounter++;
		}
		else{//tickCounter == stepSize
			tickCounter = 0;//reset tickCounter
			ledsOn();
			if (trend == dim){
				dutyCycle -= 2;
			}
			else{
				dutyCycle += 2;
			}
			printf("\ndutyCycle: %d", dutyCycle);
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



