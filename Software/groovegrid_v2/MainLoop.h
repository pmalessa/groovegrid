/*
 * MainLoop.h
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

#include "Animation/AnimationRunner.h"
#include "PLATFORM.h"
#include "driver/COMM.h"
#include "driver/GridTile.h"
#include "driver/Timer.h"
#include "utils/TaskScheduler.h"

#include "Game/Game_2048.h"
#include "Game/DisguiseGame.h"
#include "Game/Snake.h"

class MainLoop : public CommInterface
{
public:
	static MainLoop& getInstance();
	~MainLoop(void);
    std::string onUserRead(uint8_t channelID);
    void onUserWrite(std::string data, uint8_t channelID);
	void loop();

private:
	MainLoop();
	MainLoop(const MainLoop&);
	MainLoop & operator = (const MainLoop &);

	char input;
	uint8_t programState;
	GrooveGame *currentGame;
	AnimationRunner *animationRunner;
	GridTile* mainTile;
};


#endif /* MAINLOOP_H_ */
