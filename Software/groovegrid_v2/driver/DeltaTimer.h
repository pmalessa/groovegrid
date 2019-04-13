/*
 * DeltaTimer.h
 *
 *  Created on: 13.04.2019
 *      Author: pmale
 */

#ifndef DRIVER_DELTATIMER_H_
#define DRIVER_DELTATIMER_H_

#include "Arduino.h"
#include "Timer.h"

class DeltaTimer
{
public:
	DeltaTimer();
	void setTimeStep(uint32_t milliseconds);
	bool isTimeUp();
private:
	uint32_t startTime;
	uint32_t timeStep;
};


#endif /* DRIVER_DELTATIMER_H_ */
