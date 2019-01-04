#include "Arduino.h"

#include <TimerOne.h>

#include "LED.h"

//The setup function is called once at startup of the sketch




void setup()
{
	Timer1.initialize();
	Serial.begin(9600);
	Serial.println("Hey!");

	Timer1.attachInterrupt(timer, 1000);
}

// The loop function is called in an endless loop
void loop()
{
}

void timer()	//called every 1ms
{
	LED_vTimer();
}
