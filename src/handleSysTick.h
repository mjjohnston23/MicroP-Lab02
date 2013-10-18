#ifndef HANDLESYSTICK_H
#define HANDLESYSTICK_H


/**
 * This variable will let us know when the interrupt occured by the SYSTICK timer.
 *
 * We use volatile here so that the compiler does not try optimize the code by implicitly saying that this variable
 * can be modified elsewhere in the system. This is important since the SysTick_Handler() will actually access this
 * variable.
*/
extern volatile uint32_t tick;
extern const uint32_t acquiringFreq; //Sampling Rate is 20 Hz

void resetTick();
void SysTick_Handler();

#endif
