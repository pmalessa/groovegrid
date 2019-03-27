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

class Groovegrid_App_State
{
public:
	Groovegrid_App_State();
	virtual char* exportJSON() = 0;
	virtual void importJSON(char *json) = 0;
	virtual uint16_t getProgress() = 0;	//return Progress 0..100
private:
};

class Groovegrid_App : public Task
{
public:
	Groovegrid_App();
	void run();			//has to be overridden by AppLoop
	void start();		//starts App by attaching to TaskScheduler
	void pause();		//pauses App by detaching to TaskScheduler
	void stop();		//stops App by attaching to TaskScheduler and (maybe) saving GameState
	void reset(); 		//reset App to initial state by recreating AppState
protected:
	Groovegrid_App_State* state;
private:
};


#endif /* UTILS_GROOVEGRIDAPP_H_ */
