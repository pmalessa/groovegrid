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

	if(input == '0')
	{
		removeApp(currentAppID);
		AppEntry *entry = new AppEntry();
		entry->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
		entry->runningApp = new AnimationRunner(entry->tile);
		currentAppID = addApp(entry);
	}
	if(input == '1')
	{
		removeApp(currentAppID);
		AppEntry *entry = new AppEntry();
		entry->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
		entry->runningApp = new Game_2048(entry->tile);
		currentAppID = addApp(entry);
	}
	if(input == '2')
	{
		removeApp(currentAppID);
		AppEntry *entry = new AppEntry();
		entry->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
		entry->runningApp = new DisguiseGame(entry->tile);
		currentAppID = addApp(entry);
	}
	if(input == '3')
	{
		removeApp(currentAppID);
		AppEntry *entry = new AppEntry();
		entry->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
		entry->runningApp = new FlappyGroove(entry->tile);
		currentAppID = addApp(entry);
	}
	if(input == '4')
	{
		removeApp(currentAppID);
		AppEntry *entry = new AppEntry();
		entry->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
		entry->runningApp = new SnakeGame(entry->tile);
		currentAppID = addApp(entry);
	}
	if(input == 'x')	//reset
	{
		resetApp(currentAppID);
	}
}

MainLoop::~MainLoop(){}
MainLoop::MainLoop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	Timer::start();
	tsched.Attach(&comm);
	comm.Attach(this, CHANNEL_CONTROL);

	//Start initial App
	AppEntry *entry = new AppEntry();
	entry->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
	entry->runningApp = new AnimationRunner(entry->tile);
	currentAppID = addApp(entry);
}

uint8_t MainLoop::addApp(AppEntry *entry)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	runningAppList.push_back(entry);
	uint8_t appID = runningAppList.size()-1;
	entry->runningApp->start();
	tsched.Attach(entry->runningApp);
	comm.Attach(entry->runningApp, CHANNEL_USER1);
	return appID;	//return position in Vector as appID, which is the same as the size-1 directly after creating
}

void MainLoop::removeApp(uint8_t appID)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	if(appID < runningAppList.size())	//check if value in range
	{
		AppEntry *entry = runningAppList.at(appID);
		runningAppList.erase(runningAppList.begin()+appID);
		entry->runningApp->stop();
		tsched.Detach(entry->runningApp);
		comm.Detach(entry->runningApp);
		delete entry->runningApp;
		delete entry->tile;
		delete entry;
	}
}

void MainLoop::resetApp(uint8_t appID)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	AppEntry *entry = runningAppList.at(appID);
	tsched.Detach(entry->runningApp);
	comm.Detach(entry->runningApp);
	GrooveApp *newApp = entry->runningApp->new_instance(entry->tile);
	delete entry->runningApp;
	entry->runningApp = newApp;
	entry->runningApp->start();
	tsched.Attach(entry->runningApp);
	comm.Attach(entry->runningApp, CHANNEL_USER1);
}

void MainLoop::loop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	tsched.handleTasks();
	static Grid& grid = Grid::getInstance();
	grid.endWrite();
}
