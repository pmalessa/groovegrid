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
#include "MessageService.h"
#include "driver/GrooveWeb.h"

#include "Animation/BootTransition.h"
#include "utils/ProvisionApp.h"
#include "AppMap.h"

class MainLoop
{
public:
	static void start();
private:
	static bool startApp(std::string appName);
	static bool stopApp();
	static void onCommand(MessageService::CommandMsg &msg);
	static void run();

	static GrooveApp *currentApp;
	static GridTile *currentAppTile;
	static bool currentAppRunning;
	static std::string currentAppName;
	static xTaskHandle appTaskHandle;
};
#endif /* MAINLOOP_H_ */
