/*
 * MainLoop.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "MainLoop.h"

MainLoop& MainLoop::getInstance()
{
	static MainLoop _instance;
	return _instance;
}

std::string MainLoop::onUserRead(uint8_t channelID)
{
	UNUSED(channelID);
	return "bar";
}
void MainLoop::onUserWrite(std::string data, uint8_t channelID)
{
	UNUSED(data);
	UNUSED(channelID);
	input = data[0];
}

MainLoop::~MainLoop(){}
MainLoop::MainLoop()
{
	input = 0;
	programState = 0;

	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	mainTile = new GridTile(0, 0, XMAX, YMAX);
	game_2048 = new Game_2048(mainTile);
	animationRunner = new AnimationRunner(mainTile);
	disguiseGame = new DisguiseGame(mainTile);

	Timer::start();

	tsched.Attach(&comm);
	comm.Attach(this, CHANNEL_CONTROL);
	tsched.Attach(animationRunner);
	animationRunner->start();
}

void MainLoop::loop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	tsched.handleTasks();

	switch (programState) {
		case 0:	//ANIMATION

			if(input == '1')
			{
				animationRunner->stop();
				tsched.Detach(animationRunner);
				input = 0;
				programState = 1;
				//ANIMATION_vBoot();
				game_2048->start();
				comm.Attach(game_2048, CHANNEL_USER1);	//attach input to app
				tsched.Attach(game_2048);
			}
			if(input == '2')
			{
				animationRunner->stop();
				tsched.Detach(animationRunner);
				input = 0;
				programState = 2;
				comm.Attach(disguiseGame, CHANNEL_USER1);	//attach input to app
				tsched.Attach(disguiseGame);
			}
			break;
		case 1:
			if(input == 'q')
			{
				input = 0;
				tsched.Detach(game_2048);
				tsched.Attach(animationRunner);
				animationRunner->start();
				programState = 0;//quit
			}
			if(input == 'x')
			{
				input = 0;
				//ANIMATION_vBoot();
				//game_2048->reset(); change to destroying
				game_2048->start();
			}
			break;
		case 2:
			if(input == 'q')
			{
				input = 0;
				tsched.Detach(disguiseGame);
				tsched.Attach(animationRunner);
				animationRunner->start();
				programState = 0;//quit
			}
			if(input == 'x')
			{
				input = 0;
				//ANIMATION_vBoot();
				//disguiseGame->reset(); change to destroying
			}
			break;
		default:
			break;
	}
}
