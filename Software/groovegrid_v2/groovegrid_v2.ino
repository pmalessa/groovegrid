#include "Arduino.h"

#include <TimerOne.h>
#include "ANIMATION.h"
#include "LED.h"
#include "BUTTON.h"
#include "GameState.h"

uint8_t gamestate = 0;
GameState game = GameState();

void setup()
{
	Timer1.initialize();
	LED_vInit();
	BUTTON_vInit();
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
	switch (gamestate) {
		case 0:	//ANIMATION
			if(BUTTON_bIsPressed(BUTTON_UP))
			{
				gamestate = 1;
				ANIMATION_vBoot();
			}
			break;
		case 1:
			break;
		default:
			break;
	}
}

void timer()	//called every 1ms
{
	static uint16_t button_cnt = 0;
	static uint16_t game_cnt = 0;
	switch (gamestate) {
		case 0: //ANIMATION
			ANIMATION_vRunner();
			break;
		case 1:
			game_cnt++;
			if(game_cnt > 1000)
			{
				game_cnt = 0;
				gameLoop();
			}
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

void gameLoop() {
	LED_vDrawBoard(game.board);
	LED_vShow();
	while(!game.moveRight()) {
		LED_vDrawBoard(game.board);
		LED_vShow();
		delay(200);
	}
}
