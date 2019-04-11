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

class GrooveAnimation : public GrooveApp
{
	//An Animation does not need to implement all functions of a GroovegridApp
public:
	virtual ~GrooveAnimation(){};
	virtual void run() = 0;					//has to be overridden by Animation loop
	void onInput(char *data){UNUSED(data);};
};


#endif /* UTILS_GROOVEANIMATION_H_ */
