/*
 * PLATFORM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "Arduino.h"

// PLATFORM DEFINITION
#define DOOR16
//#define TABLE

#if defined(DOOR16)
#define XMAX 4
#define YMAX 4
#define DATA_PIN 6
#elif defined(TABLE)
#define XMAX 42
#define YMAX 2
#define DATA_PIN 32
#endif


#endif /* PLATFORM_H_ */
