/*
 * TIMER.c
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "Timer.h"

#if defined(__AVR__)
#include "TimerOne/TimerOne.h"
#elif defined(ESP32)
#include "esp32-hal-timer.h"
#endif

//in c++, static variables have to be declared in cpp file as well...
volatile uint32_t Timer::millisCounter;
hw_timer_t * Timer::hwtimer;

void Timer::start()
{
	millisCounter = 0;
#if defined(__AVR__)
	Timer1.initialize();
	Timer1.attachInterrupt(isr, 1000);	//1ms
#elif defined(ESP32)
	  hwtimer = timerBegin(0, 80, true);
	  timerAttachInterrupt(hwtimer, isr, true);	//call isr function
	  timerAlarmWrite(hwtimer, 1000, true);		//1ms
	  timerAlarmEnable(hwtimer);
#endif
}

void Timer::isr()
{
	millisCounter++;
}

uint32_t Timer::getMillis()
{
	return millisCounter;
}
