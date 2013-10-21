/**
 * Authors: M. Johnston and J.S. Dery
 * Handles temp conversion and temp status
*/
#include "tempStatus.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "tempFilter.h"
#include "adc.h"

uint32_t prevTemp = 0;
int32_t deviation = 0;

/**
 * Variables defined (Datasheet p.134) in the temperature formula (Reference Manual p.230) user to convert the
 * output from the ADC to actual temperature.
*/
const float avg_Slope = 0.0025;
const float v25 = 0.76;
const uint32_t samplingTime = 15;
const uint32_t celsiusBase = 25;

/**
 * Returns the value in celsius based on the 12-bit input (taken from the ADC).
 * This formula can be found in the reference manual at page 230.
*/
uint32_t getCelsius(uint16_t adcIn) {
	return (((3*((float)adcIn / 4096) - v25) / avg_Slope) + celsiusBase);
}

/**
 * compares the current temp versus the average of the filter, updates the trend if necessary
*/
void checkTempStatus(){
	// We compute the filter average once and use the computed value elsewhere in the code.
	uint32_t actualFilteredAverage = getFilterAvg();
	
	// If we observe that the computed value is different than the previous average, we update the deviation properly.
	if (prevFilterAvg < actualFilteredAverage){
		tempStatus = up;
		deviation += actualFilteredAverage - prevFilterAvg;
	} else if (prevFilterAvg > actualFilteredAverage){
		tempStatus = down;
		deviation -= prevFilterAvg - actualFilteredAverage;
	}
	else{
		tempStatus = stable;
	}
	
	//keep these printf statements to verify filter average in real time
	printf("\nFilterAvg: %d", actualFilteredAverage);
	printf("\nStatus: %d", tempStatus);
	printf("\nDeviation: %d", deviation);
}


