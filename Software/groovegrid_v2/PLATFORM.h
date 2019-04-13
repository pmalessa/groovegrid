/*
 * PLATFORM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "Arduino.h"

#define FRAMERATE_MS 33	//33ms per frame -> 30fps

// PLATFORM DEFINITION
//#define DOOR16
#define TABLE

#if defined(DOOR16)
#define XMAX 4
#define YMAX 4
#define DATA_PIN 6
#elif defined(TABLE)
#define XMAX 4
#define YMAX 4
#define DATA_PIN 32
#endif

#define UNUSED(x) (void)(x)

#endif /* PLATFORM_H_ */
