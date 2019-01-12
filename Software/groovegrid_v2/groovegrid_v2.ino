#include "Arduino.h"

#include <TimerOne.h>
#include "LED.h"
#include "BUTTON.h"
#include "HardwareSerial.h"

#include "ANIMATION.h"
#include "Game_2048.h"

uint8_t programstate = 0;
void setup()
{
	Timer1.initialize();
	LED_vInit();
	BUTTON_vInit();
	Serial.begin(9600);
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
	switch (programstate) {
		case 0:	//ANIMATION
			if(BUTTON_bIsPressed(BUTTON_UP) || Serial.read() == '1')
			{
				programstate = 1;
				ANIMATION_vBoot();
				Game_2048_vSetup();
			}
			break;
		case 1:
			if(!Game_2048_u8Loop())
			{
				programstate = 0;//quit
			}
			break;
		default:
			break;
	}
}

void timer()	//called every 1ms
{
	static uint16_t button_cnt = 0;
	switch (programstate) {
		case 0: //ANIMATION
			ANIMATION_vRunner();
			break;
		case 1:
			Game_2048_vSyncTask();
			break;
		default:
			break;
	}
	button_cnt++;
	if(button_cnt > 100)	//every 100ms
	{
		button_cnt = 0;
		BUTTON_vRead();		//read Buttons
	}
}
