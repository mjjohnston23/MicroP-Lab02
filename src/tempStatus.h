#include "stm32f4xx_conf.h"

#ifndef TEMPSTATUS_H
#define TEMPSTATUS_H




uint32_t getCelsius(uint32_t vSense);

extern void checkTempStatus();

#endif
