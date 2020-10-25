/*
 * PLATFORM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "GRIDCONFIG.h"

#include <map>
#include <functional>

#define FRAMERATE_MS 33	//33ms per frame -> 30fps


#if defined(DOOR16)
	#define GRID_WIDTH 4
	#define GRID_HEIGHT 4
	#define GRID_DATA_PIN 0
	#define BT_NAME "GrooveGrid Door"
#elif defined(ROVER)
	#define GRID_WIDTH 42
	#define GRID_HEIGHT 14
	#define NUM_STRIPS 7
	#define STRIP_WIDTH 2
	#define NUM_LEDS_PER_CHANNEL STRIP_WIDTH*GRID_WIDTH
	#define GRID_DATA1_PIN 0
	#define GRID_DATA2_PIN 18
	#define GRID_DATA3_PIN 2
	#define GRID_DATA4_PIN 5
	#define GRID_DATA5_PIN 4
	#define GRID_DATA6_PIN 17
	#define GRID_DATA7_PIN 16
	#define BT_NAME "GrooveGrid Rover"
#elif defined(TABLE)
	#define GRID_WIDTH 23
	#define GRID_HEIGHT 15
	#define NUM_STRIPS 8
	#define STRIP_WIDTH 2
	#define NUM_LEDS_PER_CHANNEL STRIP_WIDTH*GRID_WIDTH
	#define GRID_DATA1_PIN 4
	#define GRID_DATA2_PIN 5
	#define GRID_DATA3_PIN 18
	#define GRID_DATA4_PIN 19
	#define GRID_DATA5_PIN 21
	#define GRID_DATA6_PIN 27
	#define GRID_DATA7_PIN 26
	#define GRID_DATA8_PIN 25
	#define BT_NAME "GrooveGrid Table"
#endif

#define CONFIG_WEB_UPDATE_URL "https://groovegrid.de/api/device/update.php"
#define CONFIG_WEB_BEACON_URL "https://groovegrid.de/api/device/beacon.php"
#define CONFIG_WEB_INIT_URL "https://groovegrid.de/api/device/create.php"
#define CONFIG_OTA_INTERVAL_S 300		//every 5 minutes
#define CONFIG_BEACON_INTERVAL_S 30		//every 30 seconds

#define UNUSED(x) (void)(x)

#endif /* PLATFORM_H_ */
