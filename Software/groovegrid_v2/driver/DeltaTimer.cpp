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

void DeltaTimer::setTimeStep(int32_t milliseconds)
{
	timeStep = milliseconds;
}

bool DeltaTimer::isTimeUp()
{
	if(timeStep == -1)
	{
		return false; //endless
	}
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
