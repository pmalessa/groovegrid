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
	virtual void run() = 0;					//has to be overridden by AppLoop
	virtual void start() = 0;				//starts App
	virtual void pause() = 0;				//pauses App
	virtual void stop() = 0;				//stops App
	virtual void reset() = 0; 				//reset App to initial state
	virtual char* exportAppState() = 0;
	virtual void importAppState(char *input) = 0;
	virtual uint8_t getProgress() = 0;		//return Progress 0..100
protected:
	GridTile* tile;							//has to be initialized
};


#endif /* UTILS_GROOVEGRIDAPP_H_ */
