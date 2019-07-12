/*
 * TIMER.c
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "Timer.h"
#include "esp32-hal-timer.h"

//in c++, static variables have to be declared in cpp file as well...
volatile uint32_t Timer::millisCounter;
hw_timer_t * Timer::hwtimer;

void Timer::start()
{
	millisCounter = 0;
	hwtimer = timerBegin(0, 80, true);
	timerAttachInterrupt(hwtimer, isr, true);	//call isr function
	timerAlarmWrite(hwtimer, 1000, true);		//1ms
	timerAlarmEnable(hwtimer);
}

void Timer::isr()
{
	millisCounter++;
}

uint32_t Timer::getMillis()
{
	return millisCounter;
}
