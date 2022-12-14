/*
 * Transition.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef UTILS_GROOVETRANSITION_H_
#define UTILS_GROOVETRANSITION_H_

#include "../PLATFORM.h"
#include "GrooveAnimation.h"

//A Transition is an Animation, that is not endless
class GrooveTransition : public GrooveAnimation
{
public:
	GrooveTransition(GridTile *tile):GrooveAnimation(tile){};
	bool isRunning(){return running;};	//returns if Transition is still running
protected:
	bool running = false;
};




#endif /* UTILS_GROOVETRANSITION_H_ */
