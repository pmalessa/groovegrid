/*
 * MainLoop.h
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

#include "Animation/AnimationRunner.h"
#include "Animation/SpectrumAnimation.h"
#include "PLATFORM.h"
#include "driver/GridTile.h"
#include "driver/Timer.h"
#include "utils/TaskScheduler.h"
#include <map>
#include "driver/BluetoothService.h"

#include "Game/Game_2048.h"
#include "Game/DisguiseGame.h"
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
		String appName;
		GridTile *tile;
		GrooveApp *runningApp;
	};
	void startApp(String appName);
	void stopApp();
	void resetApp();
	void generateAvailableAppsMap();
	void onCommand(DynamicJsonDocument doc, uint8_t channelID);

	MainLoop();
	MainLoop(const MainLoop&);
	MainLoop & operator = (const MainLoop &);

	AppEntry *currentApp;
};


#endif /* MAINLOOP_H_ */
