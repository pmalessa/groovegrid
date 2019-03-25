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

Timer& Timer::getInstance()
{
	static Timer _instance;
	return _instance;
}

Timer::~Timer(){}
Timer::Timer()
{
	call_period = 1000;
#if defined(__AVR__)
	Timer1.initialize();
	Timer1.attachInterrupt(isr, microseconds);
#elif defined(ESP32)
	  timer = timerBegin(0, 80, true);
	  timerAttachInterrupt(timer, onEvent, true);	//call onEvent function
	  timerAlarmWrite(timer, call_period, true);
	  timerAlarmEnable(timer);
#endif
}

void Timer::setGlobalCallPeriod(uint32_t microseconds)
{
#if defined(__AVR__)
	Timer1.	//Todo
	Timer1.attachInterrupt(isr, microseconds);
#elif defined(ESP32)
	  timerAlarmDisable(timer);
	  timerAlarmWrite(timer, call_period, true);
	  timerAlarmEnable(timer);
#endif
}
