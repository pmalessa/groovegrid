/*
 * PLATFORM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "Arduino.h"
#include "CONFIG.h"

#include <map>
#include <functional>

#define FRAMERATE_MS 33	//33ms per frame -> 30fps
#define FRAMERATE_TICKS (FRAMERATE_MS*0.1)
#define MS_TO_TICKS (0.1)


#if defined(DOOR16)
	#define GRID_WIDTH 4
	#define GRID_HEIGHT 4
	#define GRID_DATA_PIN 0
#elif defined(TABLE)
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
#endif

#define UNUSED(x) (void)(x)

#endif /* PLATFORM_H_ */
