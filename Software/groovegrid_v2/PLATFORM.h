/*
 * PLATFORM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "Arduino.h"


#if defined(AVR)
#define XMAX 4
#define YMAX 4
#define DATA_PIN 6
#elif defined(ESP32)
#define XMAX 4
#define YMAX 4
#define DATA_PIN 32
#endif


#endif /* PLATFORM_H_ */
