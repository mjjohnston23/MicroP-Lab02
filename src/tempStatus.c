#include "tempStatus.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "tempFilter.h"
#include "adc.h"


/**
 * Variables defined (Datasheet p.134) in the temperature formula (Reference Manual p.230) user to convert the
 * output from the ADC to actual temperature.
*/
const float avg_Slope = 2.5;
const float v25 = 0.76;
const uint32_t samplingTime = 15;
const uint32_t celsiusBase = 25;

/**
 * Returns the value in celsius based on the voltage input (taken from the ADC).
 * This formula can be found in the reference manual at page 230.
*/
uint32_t getCelsius(uint32_t vSense) {
	return ((((uint32_t)vSense - v25) / avg_Slope) + celsiusBase);
}


/**
 * compares the current temp versus the average of the filter, updates the trend if necessary
*/
void checkTempStatus(){
	if (temperatureInCelsius > (1.1 * getFilterAvg())){
		tempStatus = up;
	}
	else if (temperatureInCelsius < (0.9 * getFilterAvg())){
		tempStatus = down;
	}
	else{
		tempStatus = stable;
	}
	printf("\nStatus: %d", tempStatus);
}
