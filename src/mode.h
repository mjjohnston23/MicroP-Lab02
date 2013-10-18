#ifndef MODE_H
#define MODE_H

#include "stm32f4xx_conf.h"

enum mode{temp, pulse};
extern enum mode activeMode;

void initializeButton(void);
void initializeMode(void);
void toggleMode(void);
void debounce(void);
void buttonPushed(void);

#endif