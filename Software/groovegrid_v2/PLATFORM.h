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
	#define XMAX 4
	#define YMAX 4
#elif defined(TABLE)
	#define XMAX 42
	#define YMAX 14
#endif

#define GRID_DATA_PIN 0

#define UNUSED(x) (void)(x)

#endif /* PLATFORM_H_ */
