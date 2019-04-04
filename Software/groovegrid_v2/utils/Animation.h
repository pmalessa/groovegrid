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
	void onInput(char *data){UNUSED(data);};
};


#endif /* UTILS_ANIMATION_H_ */
