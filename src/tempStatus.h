#include "stm32f4xx_conf.h"

#ifndef TEMPSTATUS_H
#define TEMPSTATUS_H




uint32_t getCelsius(uint16_t adcIn);

extern void checkTempStatus(void);
extern int32_t deviation;

#endif
