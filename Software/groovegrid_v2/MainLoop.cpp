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

MainLoop::~MainLoop(){}
MainLoop::MainLoop()
{
	input = 0;
	programState = 0;

	Timer::start();

	game_2048 = new Game_2048();
	mainTile = new GridTile(0, 0, XMAX, YMAX);
	animationRunner = new AnimationRunner(mainTile);

	static COMM& comm = COMM::getInstance();
	static TaskScheduler& tsched = TaskScheduler::getInstance();

	tsched.Attach(&comm);
	comm.Attach(this, COMM::MAIN);
	tsched.Attach(animationRunner);
	animationRunner->start();
}

void MainLoop::onInput(char* data)
{
	input = *data;
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
				comm.Attach(game_2048, COMM::APP);	//attach input to app
				tsched.Attach(game_2048);
			}
			break;
		case 1:
			if(input == 'q')
			{
				input = 0;
				tsched.Detach(game_2048);
				comm.Detach(game_2048, COMM::APP);	//detach input to app
				tsched.Attach(animationRunner);
				animationRunner->start();
				programState = 0;//quit
			}
			if(input == 'x')
			{
				input = 0;
				//ANIMATION_vBoot();
				game_2048->reset();
				game_2048->start();
			}
			break;
		default:
			break;
	}
}
