/*
 * Game.h
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#ifndef UTILS_GROOVEGRIDAPP_H_
#define UTILS_GROOVEGRIDAPP_H_

#include "../PLATFORM.h"
#include "Task.h"
#include "InputListener.h"
#include "../driver/GridTile.h"

class GroovegridApp : public Task,public InputListener
{
public:
	virtual ~GroovegridApp(){};
	virtual void run() = 0;					//has to be overridden by AppLoop
	char* exportAppState(){return 0;};
	void importAppState(char *input){UNUSED(input);};
	uint8_t getProgress(){return 0;};		//return Progress 0..100
protected:
	GridTile* tile;							//has to be initialized
};


#endif /* UTILS_GROOVEGRIDAPP_H_ */
