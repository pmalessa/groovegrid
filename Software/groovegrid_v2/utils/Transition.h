/*
 * Transition.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef UTILS_TRANSITION_H_
#define UTILS_TRANSITION_H_

#include "../PLATFORM.h"
#include "Animation.h"

//A Transition is an Animation, that is not endless
class Transition : public Animation
{
public:
	bool isRunning(){return running;};	//returns if Transition is still running
	void start(){running = true;}
	void stop(){running = false;}
protected:
	bool running = false;
};




#endif /* UTILS_TRANSITION_H_ */
