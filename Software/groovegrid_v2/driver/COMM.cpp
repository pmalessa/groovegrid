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

COMM& COMM::getInstance()
{
	static COMM _instance;
	return _instance;
}

COMM::~COMM(){}
COMM::COMM()
{
#if defined(__AVR__)
	Serial.begin(9600);
#elif defined(ESP32)

#endif
}

void  setCallback(void *functionPointer, COMM::Event eventType)
{

}

void  removeCallback(void *functionPointer)
{

}

//Call repeatedly in loop to process input
int COMM::read()
{
	return Serial.read();
}
