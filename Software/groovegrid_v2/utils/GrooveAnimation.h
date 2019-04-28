/*
 * Animation.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef UTILS_GROOVEANIMATION_H_
#define UTILS_GROOVEANIMATION_H_

#include "../PLATFORM.h"
#include "GrooveApp.h"
#include "../driver/DeltaTimer.h"

class GrooveAnimation : public GrooveApp
{
	//An Animation does not need to implement all functions of a GroovegridApp
public:
	GrooveAnimation(GridTile *tile):GrooveApp(tile){frameTimer.setTimeStep(FRAMERATE_MS);};
	virtual ~GrooveAnimation(){};
	void start(){};	//stub start
	void stop(){};	//stub stop
	virtual void run() = 0;					//has to be overridden by Animation loop
protected:
	DeltaTimer frameTimer;
};


#endif /* UTILS_GROOVEANIMATION_H_ */
