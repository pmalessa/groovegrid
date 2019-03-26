/*
 * TIMER.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../PLATFORM.h"

class Timer{
public:
	static uint32_t getMillis();
	static void start();
	static void stop();
 private:
	Timer();
	Timer(const Timer&);
	Timer & operator = (const Timer &);
	static void isr();

	static volatile uint32_t millisCounter;
	static hw_timer_t * hwtimer;
};

#endif /* TIMER_H_ */
