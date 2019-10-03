/*
 * MainLoop.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "MainLoop.h"

std::map<std::string, std::function<GrooveApp*(GridTile*)>> AppMap::appMap = {
		{"AnimationRunner"	,[](GridTile *tile){return new AnimationRunner(tile);}},
		{"2048"				,[](GridTile *tile){return new Game_2048(tile);}},
		{"Flappy Groove"	,[](GridTile *tile){return new FlappyGroove(tile);}},
		{"Snake"			,[](GridTile *tile){return new SnakeGame(tile);}}
	};

MainLoop& MainLoop::getInstance()
{
	static MainLoop _instance;
	return _instance;
}

void MainLoop::onCommand(CommandMsg *msg)
{
	static BluetoothService& btService = BluetoothService::getInstance();
	uint8_t errorCode = 0;

	std::string cmd = (*msg->doc)["cmd"].as<std::string>();
	if(cmd=="start")
	{
		std::string appName = (*msg->doc)["app"].as<std::string>();
		if(AppMap::appMap.find(appName) != AppMap::appMap.end())	//if name inside map
		{
			stopApp();
			startApp(appName);
			if(appName=="AnimationRunner")
			{
				std::string animation = (*msg->doc)["options"]["animation"].as<std::string>();
				if(animation!="")	//pass first animation to AnimationRunner
				{
					AnimationRunner *app = (AnimationRunner*) currentApp->runningApp;
					app->setAnimation(animation);
				}
			}
		}
		else
		{
			errorCode = 2;
		}
	}
	else if(cmd=="reset")
	{
		resetApp();
	}
	else if(cmd=="load")
	{
		currentApp->runningApp->load(msg->doc); //put Savegame
	}
	else if(cmd=="save")
	{
		(*msg->rspdoc)["name"] = currentApp->appName;
		currentApp->runningApp->save(msg->rspdoc);	//get Savegame
	}
	else if(cmd=="getGridData")
	{
		JsonObject gridData = (*msg->rspdoc).createNestedObject("data");
		gridData["height"] = GRID_HEIGHT;
		gridData["width"] = GRID_WIDTH;
	}
	else if(cmd =="getGames")
	{
		JsonArray gameList = (*msg->rspdoc).createNestedArray("list");
		for ( const auto &p : AppMap::appMap )	//iterate through map
		{
		   gameList.add(p.first);
		}
	}
	else if(cmd =="getAnimations")
	{
		JsonArray animationList = (*msg->rspdoc).createNestedArray("list");
		for ( const auto &p : AnimationMap::animationMap )	//iterate through map
		{
		   animationList.add(p.first);
		}
	}
	else if(cmd=="connect")
	{

	}
	else if(cmd=="disconnect")
	{

	}
	else
	{
		errorCode = 1;
	}
	(*msg->rspdoc)["error"]= errorCode;					//add errorCode
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
	startApp("AnimationRunner");
}

void MainLoop::stopApp()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	currentApp->runningApp->stop();
	tsched.Detach(currentApp->runningApp);
	delete currentApp->runningApp;
}

void MainLoop::startApp(std::string appName)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static BluetoothService& btService = BluetoothService::getInstance();

	if(AppMap::appMap.find(appName) != AppMap::appMap.end())
	{
		currentApp->runningApp = AppMap::appMap.at(appName).operator()(currentApp->tile);	//create new instance
	}
	else
	{
		return;
	}
	currentApp->runningApp->start();
	tsched.Attach(currentApp->runningApp);
	btService.Attach(currentApp->runningApp, CHANNEL_USER1);	//Attach CommInterface
	btService.Attach(currentApp->runningApp, CHANNEL_USER2);
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
