/*
 * DeltaTimer.cpp
 *
 *  Created on: 13.04.2019
 *      Author: pmale
 */

#include "DeltaTimer.h"

DeltaTimer::DeltaTimer()
{
	startTime = Timer::getMillis();
	timeStep = 0;
	forcedTimeUp = false;
}

DeltaTimer::DeltaTimer(int32_t milliseconds)
{
	startTime = Timer::getMillis();
	timeStep = milliseconds;
	forcedTimeUp = false;
}

void DeltaTimer::setTimeStep(int32_t milliseconds)
{
	reset();
	timeStep = milliseconds;
}

void DeltaTimer::reset()
{
	startTime = Timer::getMillis();
}

void DeltaTimer::forceTimeUp()
{
	forcedTimeUp = true;
}

bool DeltaTimer::isTimeUp()
{
	if(timeStep == -1)
	{
		return false; //endless
	}
	if(Timer::getMillis()-startTime > timeStep || forcedTimeUp == true)
	{
		forcedTimeUp = false;
		startTime = Timer::getMillis();
		return true;
	}
	else
	{
		return false;
	}
}
