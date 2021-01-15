/*
 * DeltaTimer.h
 *
 *  Created on: 13.04.2019
 *      Author: pmale
 */

#ifndef DRIVER_DELTATIMER_H_
#define DRIVER_DELTATIMER_H_

#include "../PLATFORM.h"
#include "Timer.h"

class DeltaTimer
{
public:
	DeltaTimer();
	DeltaTimer(int32_t milliseconds);
	void setTimeStep(int32_t milliseconds);
	bool isTimeUp();
	void reset();
	void forceTimeUp();
private:
	bool forcedTimeUp;
	uint32_t startTime;
	int32_t timeStep;
};


#endif /* DRIVER_DELTATIMER_H_ */
