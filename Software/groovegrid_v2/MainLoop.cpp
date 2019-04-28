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
	currentGame = nullptr;
	animationRunner = nullptr;

	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	mainTile = new GridTile(0, 0, XMAX, YMAX);

	Timer::start();
	tsched.Attach(&comm);
	comm.Attach(this, CHANNEL_CONTROL);

	animationRunner = new AnimationRunner(mainTile);
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

				currentGame = new Game_2048(mainTile);
				currentGame->start();
				comm.Attach(currentGame, CHANNEL_USER1);	//attach input to app
				tsched.Attach(currentGame);
			}
			if(input == '2')
			{
				animationRunner->stop();
				tsched.Detach(animationRunner);
				input = 0;
				programState = 2;

				currentGame = new DisguiseGame(mainTile);
				currentGame->start();
				comm.Attach(currentGame, CHANNEL_USER1);	//attach input to app
				tsched.Attach(currentGame);
			}
			break;
		case 1: //2048
			if(input == 'q')
			{
				input = 0;
				tsched.Detach(currentGame);
				comm.Detach(currentGame);
				delete currentGame; currentGame = nullptr;

				tsched.Attach(animationRunner);
				animationRunner->start();
				programState = 0;//quit
			}
			if(input == 'x')	//reset
			{
				input = 0;
				//ANIMATION_vBoot();
				currentGame->stop();
				tsched.Detach(currentGame);
				comm.Detach(currentGame);
				delete currentGame;

				currentGame = new Game_2048(mainTile);
				currentGame->start();
				comm.Attach(currentGame, CHANNEL_USER1);
				tsched.Attach(currentGame);
			}
			break;
		case 2:	//Disguise
			if(input == 'q')
			{
				input = 0;
				tsched.Detach(currentGame);
				comm.Detach(currentGame);
				delete currentGame; currentGame = nullptr;

				tsched.Attach(animationRunner);
				animationRunner->start();
				programState = 0;//quit
			}
			if(input == 'x')
			{
				input = 0;
				//ANIMATION_vBoot();
				currentGame->stop();
				tsched.Detach(currentGame);
				comm.Detach(currentGame);
				delete currentGame;

				currentGame = new Game_2048(mainTile);
				currentGame->start();
				comm.Attach(currentGame, CHANNEL_USER1);
				tsched.Attach(currentGame);
			}
			break;
		default:
			break;
	}
}
