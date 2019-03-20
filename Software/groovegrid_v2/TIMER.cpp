/*
 * TIMER.c
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#include "TIMER.h"

#if defined(__AVR__)
#include "libraries/TimerOne/TimerOne.h"
#endif

void TIMER_Init()
{
#if defined(__AVR__)
	Timer1.initialize();
#endif
}

void TIMER_attach(void (*isr)(), unsigned long microseconds)
{
#if defined(__AVR__)
	Timer1.attachInterrupt(isr, microseconds);
#endif
}
