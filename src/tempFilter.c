/**
 * Authors: M. Johnston and J.S. Dery
 * Handles moving-average temperature filter
*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "tempFilter.h"
#include "tempStatus.h"
#include "adc.h"


const uint32_t filterDepth = 30;
uint32_t tempFilter[filterDepth];
uint32_t prevFilterAvg;

enum trend tempStatus = stable;

/**
 * Adds a temperature reading to the moving average filter, pushes elements up by one.
*/
void addToFilter(){
	prevFilterAvg = getFilterAvg(); //take average value and remember it
	int i = filterDepth;
	for (i = filterDepth; i > 0; i--){
		tempFilter[i] = tempFilter[i - 1]; 
	}
	tempFilter[0] = temperatureInCelsius;
	//printf("\n%d,%d,%d,%d,%d,%d", tempFilter[0],  tempFilter[1],  tempFilter[2],  tempFilter[3],  tempFilter[4], tempFilter[5]);
}
/**
 * Returns the average value of the moving average filter.
*/
uint32_t getFilterAvg(){
	uint32_t sum = 0;
	int i = 0;
	for (i = 0; i < filterDepth; i++){
		sum += tempFilter[i];
	}
	int average = sum / filterDepth;
	return average;
} 