/*
 * MainLoop.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "MainLoop.h"

static const char* TAG = "MainLoop";
GrooveApp *MainLoop::currentApp;
bool MainLoop::currentAppRunning;
std::string MainLoop::currentAppName;
xTaskHandle MainLoop::appTaskHandle;
GridTile *MainLoop::currentAppTile;

void MainLoop::onCommand(MessageService::CommandMsg &msg)
{
	msg.rspdoc["error"]= 0;

	std::string cmd = msg.doc["cmd"].as<std::string>();
	if(cmd=="start")
	{
		std::string appName = msg.doc["app"].as<std::string>();
		if(startApp(appName) != true)
		{
			msg.rspdoc["error"]= 2;
		}
		//TODO:pass options to app
		if(appName=="AnimationRunner")
		{
			std::string animation = msg.doc["options"]["animation"].as<std::string>();
			if(animation!="")	//pass first animation to AnimationRunner
			{
				AnimationRunner *app = (AnimationRunner*) currentApp;
				app->setAnimation(animation);
			}
		}
	}
	else if(cmd=="reset")
	{
		startApp(currentAppName);
	}
	else if(cmd=="getGridData")
	{
		JsonObject gridData = msg.rspdoc.createNestedObject("data");
		gridData["height"] = GRID_HEIGHT;
		gridData["width"] = GRID_WIDTH;
	}
	else if(cmd =="getGames")
	{
		JsonArray gameList = msg.rspdoc.createNestedArray("list");
		for ( const auto &p : appMap )	//iterate through map
		{
		   gameList.add(p.first);
		}
	}
	else if(cmd =="getAnimations")
	{
		JsonArray animationList = msg.rspdoc.createNestedArray("list");
		for ( const auto &p : animationMap )	//iterate through map
		{
		   animationList.add(p.first);
		}
	}
	else if(cmd=="brightness")
	{
		uint8_t brightness = msg.doc["value"].as<uint8_t>();
		Grid::setGlobalBrightness(brightness);
	}
	else
	{
		msg.rspdoc["error"]= 1;
	}
}

void MainLoop::start()
{
	Timer::start();
	MessageService::attachCallback(&MainLoop::onCommand);	//no need to save id, callback never removed
	//Start initial App
	currentAppTile = new GridTile(0, 0, GRID_WIDTH, GRID_HEIGHT, 0);
	startApp("AnimationRunner");

	static GrooveWeb& webService = GrooveWeb::getInstance();
	UNUSED(webService);
	xTaskCreatePinnedToCore((TaskFunction_t)run,"appTask",8192,nullptr,tskIDLE_PRIORITY,&appTaskHandle,1);
}

bool MainLoop::stopApp()
{
	if(currentApp != nullptr)
	{
		currentApp->stop();
		currentAppRunning = false;	//TODO: this is maybe not thread safe! currentApp could be running while deleting it
		delete currentApp;
		currentApp = nullptr;
	}
	else
	{
		ESP_LOGE(TAG,"current App is nullptr!");
		return false;
	}
	return true;
}

bool MainLoop::startApp(std::string appName)
{
	GrooveApp *newApp;

	if(appMap.find(appName) == appMap.end())
	{
		return false; //App not found
	}

	newApp = appMap.at(appName).operator()(currentAppTile);	//create new instance

	if(newApp == nullptr)
	{
		ESP_LOGE(TAG, "App Creation failed!");
		return false;
	}
	if(currentAppRunning == true)
	{
		if(stopApp() == false){
			ESP_LOGE(TAG, "App Stop failed!");
		}
	}
	delete currentApp;
	currentApp = newApp;
	currentApp->start();
	currentAppRunning = true;
	currentAppName = appName;
	return true;
}

void MainLoop::run()
{
	ProvisionApp *provisionApp =new ProvisionApp(currentAppTile);
	while(provisionApp->isRunning())
	{
		provisionApp->run();
		vTaskDelay(1);
	}
	delete provisionApp;
	provisionApp = nullptr;
	BootTransition *bt = new BootTransition(currentAppTile);
	while(bt->isRunning())
	{
		bt->run();
		vTaskDelay(1);
	}
	delete bt;
	bt = nullptr;
	while(1)
	{
		if(currentAppRunning)
		{
			if(currentApp != nullptr)
			{
				currentApp->run();
			}
		}
		vTaskDelay(1);
	}
}
