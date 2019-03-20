/*
 * COMM.cpp
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "COMM.h"


#if defined(__AVR__)
#include "HardwareSerial.h"
#elif defined(ESP32)
#include "HardwareSerial.h"
#endif

void COMM_Init()
{
#if defined(__AVR__)
	Serial.begin(9600);
#elif defined(ESP32)
	Serial.begin(115200);
#endif
}

void COMM_println(const char str[])
{
	Serial.println(str);
}
