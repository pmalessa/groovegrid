/*
 * Animation.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef UTILS_ANIMATION_H_
#define UTILS_ANIMATION_H_

#include "../PLATFORM.h"
#include "GroovegridApp.h"

class Animation : public GroovegridApp
{
	//An Animation does not need to implement all functions of a GroovegridApp
public:
	virtual void run() = 0;					//has to be overridden by Animation loop

	//added empty function for following functions:
	void start(){};				//starts Animation
	void stop(){};				//stops Animation
	void reset(){}; 				//reset Animation to initial state
	void pause(){stop();}					//pauses Animation, defaults to stop
	char* exportAppState(){return 0;}
	void importAppState(char *input){}
	uint8_t getProgress(){return 0;}
	void onInput(char *data){}
};


#endif /* UTILS_ANIMATION_H_ */
