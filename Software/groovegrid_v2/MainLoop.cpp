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
	static BluetoothService& btService = BluetoothService::getInstance();
	DynamicJsonDocument rspDoc(200);
	uint8_t errorCode = 0;
	rspDoc["rspID"] = doc["cmdID"];	//send cmdID back as rspID

	String cmd = doc["cmd"].as<String>();

	if(cmd=="start")
	{
		String game = doc["app"].as<String>();
		if(game=="AnimationRunner")
		{
			stopApp();
			startApp(game);
			String animation = doc["options"]["animation"].as<String>();
			if(animation!="")	//pass first animation to AnimationRunner
			{
				AnimationRunner *app = (AnimationRunner*) currentApp->runningApp;
				app->setAnimation(animation);
			}
		}
		else if(game=="Snake")
		{
			stopApp();
			startApp(game);
		}
		else if(game=="Flappy Groove")
		{
			stopApp();
			startApp(game);
		}
		else if(game=="2048")
		{
			stopApp();
			startApp(game);
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
		gameList.add("Snake");
		gameList.add("2048");
		gameList.add("FlappyGroove");
		gameList.add("Battleship");
	}
	else if(cmd =="getAnimations")
	{
		JsonArray gameList = rspDoc.createNestedArray("list");
		gameList.add("Color Palette");
		gameList.add("Matrix");
		gameList.add("Need for Speed");
		gameList.add("Simply Red");
		gameList.add("Spectrum");
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

	if(appName=="AnimationRunner")
	{
		currentApp->runningApp = new AnimationRunner(currentApp->tile);
	}
	else if(appName=="2048")
	{
		currentApp->runningApp = new Game_2048(currentApp->tile);
	}
	else if(appName=="FlappyGroove")
	{
		currentApp->runningApp = new FlappyGroove(currentApp->tile);
	}
	else if(appName=="Snake")
	{
		currentApp->runningApp = new SnakeGame(currentApp->tile);
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
