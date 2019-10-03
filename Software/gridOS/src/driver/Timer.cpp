/*
 * TIMER.c
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "Timer.h"

void Timer::start()
{
}

uint32_t Timer::getMillis()
{
	return xTaskGetTickCount();	//if 1000Hz Tick rate
}
