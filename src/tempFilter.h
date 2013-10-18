#include "stm32f4xx.h"

#ifndef FILTER_DEPTH
#define FILTER_DEPTH 10
#endif 

#ifndef TEMPFILTER_H
#define TEMPFILTER_H


void addToFilter();
uint32_t getFilterAvg();

/**
 * Variables needed for the moving average temperature filter
*/
extern const uint32_t filterDepth;
extern uint32_t tempFilter[FILTER_DEPTH];
enum trend {up, down, stable};
extern enum trend tempStatus;

#endif
