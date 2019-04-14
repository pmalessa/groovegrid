/*
 * DeltaTimer.cpp
 *
 *  Created on: 13.04.2019
 *      Author: pmale
 */

#include "DeltaTimer.h"

DeltaTimer::DeltaTimer()
{
	startTime = 0;
	timeStep = 0;
}

void DeltaTimer::setTimeStep(uint32_t milliseconds)
{
	timeStep = milliseconds;
}

bool DeltaTimer::isTimeUp()
{
	if(Timer::getMillis()-startTime > timeStep)
	{
		startTime = Timer::getMillis();
		return true;
	}
	else
	{
		return false;
	}
}
