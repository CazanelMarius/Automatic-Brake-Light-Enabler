#ifndef BREAK_LIGHT_H
#define BREAK_LIGHT_H

#include "lcd_api.h"

#define LIGHT_ACTIVATED 1

#define RED_LIGHT_DELAY 500
#define THRESHOLD - 0.05
#define BRAKE_TIME 10

extern uint32_t startTime;
extern short redLightOnTrigger; // 1 we want to turn on the red light with a delay

short checkAccForBreakLight(const double*, int, unsigned int current_iteration);
short isUnderTreshold(double);

//void sleep_ms(int);

void checkForRedLight(int);
void onRedLightDisabledWithDelay();

#endif
