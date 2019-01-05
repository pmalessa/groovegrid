#include "Arduino.h"

#include <TimerOne.h>
#include "ANIMATION.h"
#include "LED.h"

void setup()
{
	Timer1.initialize();
	LED_vInit();
	srand(eeprom_read_word((uint16_t *)0x23));
	eeprom_update_word((uint16_t *)0x23, (uint16_t)rand());
	Serial.begin(9600);
	Serial.println("Hey!");

	ANIMATION_vBoot();
	Timer1.attachInterrupt(timer, 1000);
}

// The loop function is called in an endless loop
void loop()
{
}

void timer()	//called every 1ms
{
	ANIMATION_vRunner();
}
