#include "stm32f4xx.h"

#ifndef FILTER_DEPTH
#define FILTER_DEPTH 30
#endif 

#ifndef TEMPFILTER_H
#define TEMPFILTER_H


void addToFilter(void);
uint32_t getFilterAvg(void);

/**
 * Variables needed for the moving average temperature filter
*/
extern const uint32_t filterDepth;
extern uint32_t tempFilter[FILTER_DEPTH];
enum trend {up, down, stable};
extern enum trend tempStatus;
extern uint32_t prevFilterAvg;

#endif
