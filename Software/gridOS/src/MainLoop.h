/*
 * MainLoop.h
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

#include "PLATFORM.h"

#include "driver/Timer.h"
#include "utils/TaskScheduler.h"
#include "driver/BluetoothService.h"

#include "Animation/AnimationRunner.h"
#include "Game/Game_2048.h"
#include "Game/FlappyGroove.h"
#include "Game/Snake.h"

class MainLoop : private CommInterface
{
public:
	static MainLoop& getInstance();
	~MainLoop(void);
	void loop();
private:
	class AppEntry{
	public:
		std::string appName;
		GridTile *tile;
		GrooveApp *runningApp;
	};
	void startApp(std::string appName);
	void stopApp();
	void resetApp();
	void onCommand(CommandMsg *msg);

	MainLoop();
	MainLoop(const MainLoop&);
	MainLoop & operator = (const MainLoop &);

	AppEntry *currentApp;

};

class AppMap{
public:
	static std::map<std::string, std::function<GrooveApp*(GridTile*)>> appMap;
	static void add(std::string appName, std::function<GrooveApp*(GridTile*)> returnObjectFunction)
	{
		appMap.emplace(appName,returnObjectFunction);
	};
};

#endif /* MAINLOOP_H_ */
