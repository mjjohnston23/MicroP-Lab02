#ifndef	 ADC_H
#define ADC_H

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "gpio_example.h"


void configureADC(void);
void acquireADCValue(void);

extern uint16_t ADC_output;
extern uint32_t temperatureInCelsius;
#endif
