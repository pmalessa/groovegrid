/*
 * TIMER.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../PLATFORM.h"
#include "../utils/EventManager.h"

class Timer: public EventManager{
public:
	static Timer& getInstance();
	~Timer(void);

	void setGlobalCallPeriod(uint32_t microseconds);

 private:
	Timer();
	Timer(const Timer&);
	Timer & operator = (const Timer &);

	static uint32_t call_period;	//1ms standard
	static hw_timer_t * timer;
};

#endif /* TIMER_H_ */
