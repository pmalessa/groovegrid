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
		{"Snake"			,[](GridTile *tile){return new SnakeGame(tile);}},
		{"Battleship"		,[](GridTile *tile){return new Battleship(tile);}},
		{"Swipe Master"		,[](GridTile *tile){return new SwipeMaster(tile);}}
	};

MainLoop& MainLoop::getInstance()
{
	static MainLoop _instance;
	return _instance;
}

void MainLoop::onCommand(CommandMsg *msg)
{
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
		startApp(currentApp->appName);
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
	else if(cmd=="brightness")
	{
		uint8_t brightness = (*msg->doc)["value"].as<uint8_t>();
		Grid::setGlobalBrightness(brightness);
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
	Timer::start();
	MessageService::attachCallback(this, CommInterface::CHANNEL_CONTROL);	//Attach CommInterface

	//Start initial App
	currentApp = new AppEntry();
	currentApp->tile = new GridTile(0, 0, GRID_WIDTH-1, GRID_HEIGHT-1);
	currentApp->isRunning = false;
	startApp("AnimationRunner");

	static GrooveWeb& webService = GrooveWeb::getInstance();
	UNUSED(webService);
	xTaskCreatePinnedToCore(appTaskWrapper,"appTask",8192,this,tskIDLE_PRIORITY,&appTaskHandle,1);
}

void MainLoop::stopApp()
{
	if(currentApp->runningApp != nullptr)
	{
		currentApp->runningApp->stop();
		currentApp->isRunning = false;	//TODO: this is maybe not thread safe! currentApp could be running while deleting it
		delete currentApp->runningApp;
	}
}

void MainLoop::startApp(std::string appName)
{
	if(currentApp->isRunning == true)
	{
		stopApp();
	}

	if(AppMap::appMap.find(appName) != AppMap::appMap.end())
	{
		currentApp->runningApp = AppMap::appMap.at(appName).operator()(currentApp->tile);	//create new instance
		currentApp->runningApp->start();
		currentApp->isRunning = true;
		currentApp->appName = appName;
		MessageService::attachCallback(currentApp->runningApp, CHANNEL_USER1);	//Attach CommInterface
		MessageService::attachCallback(currentApp->runningApp, CHANNEL_USER2);
		MessageService::attachCallback(currentApp->runningApp, CHANNEL_USER3);
		MessageService::attachCallback(currentApp->runningApp, CHANNEL_USER4);
	}
	else
	{
		return;
	}
}

void MainLoop::appTask()
{
	ProvisionApp *provisionApp =new ProvisionApp(currentApp->tile);
	while(provisionApp->isRunning())
	{
		provisionApp->run();
		vTaskDelay(1);
	}
	delete provisionApp;
	BootTransition *bt = new BootTransition(currentApp->tile);
	while(bt->isRunning())
	{
		bt->run();
		vTaskDelay(1);
	}
	delete bt;
	while(1)
	{
		if(currentApp->isRunning)
		{
			if(currentApp->runningApp != nullptr)
			{
				currentApp->runningApp->run();
			}
		}
		vTaskDelay(1);
	}
}
