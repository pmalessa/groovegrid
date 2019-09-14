/*
 * MainLoop.h
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

#include "PLATFORM.h"
#include <map>
#include <functional>

#include "driver/GridTile.h"
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
		String appName;
		GridTile *tile;
		GrooveApp *runningApp;
	};
	void startApp(String appName);
	void stopApp();
	void resetApp();
	void onCommand(DynamicJsonDocument doc, uint8_t channelID);

	MainLoop();
	MainLoop(const MainLoop&);
	MainLoop & operator = (const MainLoop &);

	AppEntry *currentApp;
	std::map<String, std::function<GrooveApp*(GridTile*)>> appMap = {
		{"AnimationRunner"	,[](GridTile *tile){return new AnimationRunner(tile);}},
		{"2048"				,[](GridTile *tile){return new Game_2048(tile);}},
		{"Flappy Groove"	,[](GridTile *tile){return new FlappyGroove(tile);}},
		{"Snake"			,[](GridTile *tile){return new SnakeGame(tile);}}
	};

};


#endif /* MAINLOOP_H_ */
