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

void MainLoop::onCommand(DynamicJsonDocument doc, uint8_t channelID)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	UNUSED(channelID);
	String cmd = doc["cmd"].as<String>();
	String game = doc["game"].as<String>();

	if(cmd=="start")
	{
		currentApp->runningApp->stop();
		tsched.Detach(currentApp->runningApp);
		delete currentApp->runningApp;
		if(game=="AnimationRunner")
		{
			currentApp->runningApp = new AnimationRunner(currentApp->tile);
			currentApp->runningApp->start();
			tsched.Attach(currentApp->runningApp);
		}
		else if(game=="Snake")
		{
			currentApp->runningApp = new SnakeGame(currentApp->tile);
			currentApp->runningApp->start();
			tsched.Attach(currentApp->runningApp);
		}
		else if(game=="FlappyGroove")
		{
			currentApp->runningApp = new FlappyGroove(currentApp->tile);
			currentApp->runningApp->start();
			tsched.Attach(currentApp->runningApp);
		}
		else if(game=="2048")
		{
			currentApp->runningApp = new Game_2048(currentApp->tile);
			currentApp->runningApp->start();
			tsched.Attach(currentApp->runningApp);
		}
		else if(game=="DisguiseGame")
		{
			currentApp->runningApp = new DisguiseGame(currentApp->tile);
			currentApp->runningApp->start();
			tsched.Attach(currentApp->runningApp);
		}
	}
	else if(cmd=="reset")
	{
		resetApp();
	}
}

MainLoop::~MainLoop(){}
MainLoop::MainLoop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static BluetoothService& btService = BluetoothService::getInstance();

	Timer::start();
	tsched.Attach(&btService);

	btService.Attach(this, CHANNEL_CONTROL);	//Attach CommInterface

	//Start initial App
	currentApp = new AppEntry();
	currentApp->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
	currentApp->runningApp = new AnimationRunner(currentApp->tile);
}

void MainLoop::resetApp()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();

	tsched.Detach(currentApp->runningApp);
	GrooveApp *newApp = currentApp->runningApp->new_instance(currentApp->tile);
	delete currentApp->runningApp;
	currentApp->runningApp = newApp;
	currentApp->runningApp->start();
	tsched.Attach(currentApp->runningApp);
}

void MainLoop::loop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	tsched.handleTasks();
	static Grid& grid = Grid::getInstance();
	grid.endWrite();
}
