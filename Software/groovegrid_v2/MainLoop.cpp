/*
 * MainLoop.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "MainLoop.h"

std::map<String, std::function<GrooveApp*(GridTile*)>> AppMap::appMap = {
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

void MainLoop::onCommand(DynamicJsonDocument doc, uint8_t channelID)
{
	static BluetoothService& btService = BluetoothService::getInstance();
	DynamicJsonDocument rspDoc(200);
	uint8_t errorCode = 0;
	rspDoc["rspID"] = doc["cmdID"];	//send cmdID back as rspID

	String cmd = doc["cmd"].as<String>();

	if(cmd=="start")
	{
		String appName = doc["app"].as<String>();
		if(AppMap::appMap.find(appName) != AppMap::appMap.end())	//if name inside map
		{
			stopApp();
			startApp(appName);
			if(appName=="AnimationRunner")
			{
				String animation = doc["options"]["animation"].as<String>();
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
		currentApp->runningApp->load(&doc); //put Savegame
	}
	else if(cmd=="save")
	{
		rspDoc["name"] = currentApp->appName;
		currentApp->runningApp->save(&rspDoc);	//get Savegame
	}
	else if(cmd=="getGridData")
	{
		JsonObject gridData = rspDoc.createNestedObject("data");
		gridData["height"] = GRID_HEIGHT;
		gridData["width"] = GRID_WIDTH;
	}
	else if(cmd =="getGames")
	{
		JsonArray gameList = rspDoc.createNestedArray("list");
		for ( const auto &p : AppMap::appMap )	//iterate through map
		{
		   gameList.add(p.first);
		}
	}
	else if(cmd =="getAnimations")
	{
		JsonArray animationList = rspDoc.createNestedArray("list");
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
	rspDoc["error"]= errorCode;					//add errorCode
	btService.sendResponse(rspDoc, channelID);	//send Response
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

void MainLoop::startApp(String appName)
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
