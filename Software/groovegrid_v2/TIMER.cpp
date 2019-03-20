/*
 * TIMER.c
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "TIMER.h"

#if defined(__AVR__)
#include "TimerOne/TimerOne.h"
#elif defined(ESP32)
#include "esp32-hal-timer.h"
hw_timer_t * timer = NULL;
#endif

void TIMER_Init()
{
#if defined(__AVR__)
	Timer1.initialize();
#elif defined(ESP32)
//	  timer = timerBegin(0, 80, true);
#endif
}

void TIMER_attach(void (*isr)(), unsigned long microseconds)
{
#if defined(__AVR__)
	Timer1.attachInterrupt(isr, microseconds);
#elif defined(ESP32)
//	  timerAttachInterrupt(timer, isr, true);
//	  timerAlarmWrite(timer, microseconds, true);
//	  timerAlarmEnable(timer);
#endif
}
