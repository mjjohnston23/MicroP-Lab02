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
extern uint32_t acquiringFreq; //Sampling Rate is 20 Hz for temp, we will use 1 kHz for pwm switching. 

void resetTick(void);
void SysTick_Handler(void);

#endif
