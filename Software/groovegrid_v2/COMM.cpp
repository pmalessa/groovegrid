/*
 * COMM.cpp
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#include "COMM.h"

#if defined(__AVR__)
#include "HardwareSerial.h"
#endif

void COMM_Init()
{
#if defined(__AVR__)
	Serial.begin(9600);
#endif
}

void COMM_println(const char str[])
{
#if defined(__AVR__)
	Serial.println(str);
#endif
}
