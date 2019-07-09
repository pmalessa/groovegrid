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

class MainLoop
{
public:
	static MainLoop& getInstance();
	~MainLoop(void);
    std::string onUserRead(uint8_t channelID);
    void onUserWrite(std::string data, uint8_t channelID);
	void loop();

private:
	class AppEntry{
	public:
		std::string appName;
		GridTile *tile;
		GrooveApp *runningApp;
	};

	uint8_t addApp(AppEntry *entry); //return appID
	void removeApp(uint8_t appID);
	void resetApp(uint8_t appID);
	void generateAvailableAppsMap();

	MainLoop();
	MainLoop(const MainLoop&);
	MainLoop & operator = (const MainLoop &);

	std::vector<AppEntry*> runningAppList;

	char input;
	uint8_t currentAppID;
};


#endif /* MAINLOOP_H_ */
