/*
 * MainLoop.h
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

#include "PLATFORM.h"
#include "utils/InputListener.h"
#include "driver/COMM.h"
#include "driver/GridTile.h"
#include "driver/Timer.h"
#include "utils/TaskScheduler.h"

#include "Game/Game_2048.h"
#include "Animation/GrooveAnimationRunner.h"
#include "Game/DisguiseGame.h"

class MainLoop : public InputListener
{
public:
	static MainLoop& getInstance();
	~MainLoop(void);
	void onInput(char* data);
	void loop();

private:
	MainLoop();
	MainLoop(const MainLoop&);
	MainLoop & operator = (const MainLoop &);

	char input;
	uint8_t programState;
	Game_2048* game_2048;	//change to general Game* later
	DisguiseGame* disguiseGame;
	AnimationRunner* animationRunner;
	GridTile* mainTile;
};


#endif /* MAINLOOP_H_ */
