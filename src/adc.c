#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "adc.h"
#include "tempStatus.h"
#include "tempFilter.h"
#include "leds.h"

uint16_t ADC_output;
uint32_t temperatureInCelsius;

/**
 * Configures the ADC on channel 16 so we can get data from the temperature sensor.
*/
void configureADC() {
	printf("configuring ADC");
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&adc_common_init_s);
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &adc_init_s);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_TempSensorVrefintCmd(ENABLE);	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
}


/**
 * Does shit
*/
void acquireADCValue() {
	
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Could be through interrupts (Later)
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_GetConversionValue(ADC1); // Result available in ADC1->DR
	// Gets the value from the ADC.
	ADC_output = ADC_GetConversionValue(ADC1);
	//printf("\n Output from ADC=%i", ADC_output);
	
	temperatureInCelsius = getCelsius(ADC_output);
	//printf("\n Celsius=%i", temperatureInCelsius);
	
	addToFilter();
	checkTempStatus();
	
	ledState();
}
