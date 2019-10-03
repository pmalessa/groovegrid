/*
 * Animation.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef UTILS_GROOVEANIMATION_H_
#define UTILS_GROOVEANIMATION_H_

#include "../PLATFORM.h"
#include "../driver/DeltaTimer.h"
#include "Task.h"
#include "../driver/CommInterface.h"
#include "../driver/GridTile.h"

class GrooveAnimation: public Task, public CommInterface
{
public:
	GrooveAnimation(GridTile *tile){this->tile = tile; frameTimer.setTimeStep(FRAMERATE_MS);};
	virtual ~GrooveAnimation(){};
	void start(){};	//stub start
	void stop(){};	//stub stop
	void onCommand(CommandMsg *msg){};	//can be overridden
	virtual void run() = 0;					//has to be overridden by Animation loop
protected:
	GridTile* tile;							//has to be initialized
	DeltaTimer frameTimer;
};


#endif /* UTILS_GROOVEANIMATION_H_ */
