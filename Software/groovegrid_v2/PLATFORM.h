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

#define FRAMERATE_MS 33	//33ms per frame -> 30fps

#if defined(DOOR16)
	#define GRID_WIDTH 4
	#define GRID_HEIGHT 4
#elif defined(TABLE)
	#define GRID_WIDTH 42
	#define GRID_HEIGHT 14
#endif

#define GRID_DATA_PIN 0

#define UNUSED(x) (void)(x)

#endif /* PLATFORM_H_ */
