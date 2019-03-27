/*
 * Game.h
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#ifndef UTILS_GAME_H_
#define UTILS_GAME_H_

#include "../PLATFORM.h"
#include "Task.h"

class GameState
{
public:
	virtual char* exportJSON() = 0;
	virtual void importJSON(char *json) = 0;
	virtual uint16_t getProgress() = 0;	//return Progress 0..100
private:
};

class Game : public Task
{
public:
	Game();
	void run();			//has to be overridden by GameLoop
	void start();		//starts Game by attaching to TaskScheduler
	void pause();		//pauses Game by detaching to TaskScheduler
	void stop();		//stops Game by attaching to TaskScheduler and (maybe) saving GameState
	void reset(); 		//reset Game to initial state
protected:
	GameState* state;
private:
};


#endif /* UTILS_GAME_H_ */
