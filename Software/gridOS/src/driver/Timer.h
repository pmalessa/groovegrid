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
};

#endif /* TIMER_H_ */
