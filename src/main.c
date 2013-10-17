/**
 * Main class of ECSE-426 Lab02.
*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "gpio_example.h"

/**
 * This variable will let us know when the interrupt occured by the SYSTICK timer.
 *
 * We use volatile here so that the compiler does not try optimize the code by implicitly saying that this variable
 * can be modified elsewhere in the system. This is important since the SysTick_Handler() will actually access this
 * variable.
*/
static volatile uint32_t tick;
const uint32_t acquiringFreq = 20; //Sampling Rate is 20 Hz
/**
 * Variables defined (Datasheet p.134) in the temperature formula (Reference Manual p.230) user to convert the
 * output from the ADC to actual temperature.
*/
const float avg_Slope = 2.5;
const float v25 = 0.76;
const uint32_t samplingTime = 15;
const uint32_t celsiusBase = 25;

uint32_t ADC_output;
uint32_t temperatureInCelsius;


/**
 * Variables needed for the moving average temperature filter
*/
const uint32_t filterDepth = 10;
uint32_t tempFilter[filterDepth];
enum trend {up, down, stable};
enum trend tempStatus = stable;

/**
 * Variables needed for cool LED action
*/

enum led {top, right, bottom, left};
enum led activeLed;
/**
 * Defines functions used in the main file.
*/
void resetTick();
void mainPreamble();
void acquireADCValue();
void configureADC();
uint32_t getCelsius(uint32_t vSense);
void addToFilter();
void checkTempStatus();
void startLeds();

/**
 * Main method for Lab03.
*/
int main() {
	mainPreamble();
	
	int counter = 0;
	while (1) {
		// Here we wait for the interrupt to happen.
		while (!tick);
		printf("tick %d ", counter);
		counter++;
		resetTick();
		acquireADCValue();
	
		
	}
	
	return (0);
}

/**
 * Does shit
*/
void acquireADCValue() {
	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Could be through interrupts (Later)
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_GetConversionValue(ADC1); // Result available in ADC1->DR
	// Gets the value from the ADC.
	ADC_output = ADC_GetConversionValue(ADC1);
	printf("\n Output from ADC=%i", ADC_output);
	
	temperatureInCelsius = getCelsius(ADC_output);
	printf("\n Celsius=%i", temperatureInCelsius);
	
	addToFilter();
	checkTempStatus();
}

/**
 * Returns the value in celsius based on the voltage input (taken from the ADC).
 * This formula can be found in the reference manual at page 230.
*/
uint32_t getCelsius(uint32_t vSense) {
	return ((((uint32_t)vSense - v25) / avg_Slope) + celsiusBase);
}

/**
 * Sets all the pre-conditions for the main method to execute properly.
*/
void mainPreamble() {
	// Sets tick to 0.
	resetTick();
	
	// Initialize the clocks.
	SystemInit();
	
	//Get the LEDs running
	startLeds();
		
	// Sets the SysTick delay needed to sample the ADC at the required rate.
	uint32_t result = SysTick_Config(SystemCoreClock / acquiringFreq);
	
	// Handles the error if something went wrong.
	if (result != 0) {
		printf("An error occured when setting the SYSTICK timer. Terminating the program now.");
		exit(EXIT_FAILURE);
	}
	
	configureADC();
}

/**
 * Configures the ADC on channel 16 so we can get data from the temperature sensor.
*/
void configureADC() {
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
}
/**
 * Adds a temperature reading to the moving average filter, pushes elements up by one.
*/
void addToFilter(){
	int i = filterDepth;
	for (i = filterDepth; i > 0; i--){
		tempFilter[i] = tempFilter[i - 1]; 
	}
	tempFilter[0] = temperatureInCelsius;
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
/**
 * starts the LEDs with the top one activated
*/
void startLeds(){
	activeLed = top;
	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	printf("lights on");
}

/**
 * rotates the active LED in a clockwise manner (increase in temp)
*/
void cwLeds(enum led activeLed){
	
}
	

/**
 * Resets the tick interrupt variable to 0.
*/
void resetTick() {
	tick = 0;
}

/**
 * Interrupt handler for SysTick.
 * When the actual interrupt will happen, this method will be called and its purpose
 * is to set the tick boolean to 1 (so the main method knows that the interrupt happened).
*/
void SysTick_Handler() {
	tick = 1;
}