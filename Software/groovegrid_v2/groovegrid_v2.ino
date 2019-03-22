
#include "PLATFORM.h"
#include "driver/TIMER.h"
#include "driver/BUTTON.h"
#include "driver/COMM.h"
#include "driver/GRID.h"

#include "Animation/ANIMATION.h"
#include "2048/Game_2048.h"
#include "TicTacToe/Game_TicTacToe.h"

uint8_t programstate = 0;

void setup()
{
	TIMER_Init();
	BUTTON_vInit();
	COMM_Init();
#if defined(__AVR__)
	srand(eeprom_read_word((uint16_t *)0x23));
	eeprom_update_word((uint16_t *)0x23, (uint16_t)rand());
#endif
	COMM_println("Hey!");

	ANIMATION_vBoot();
	TIMER_attach(timer, 1000);
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
		case 2:
			Game_TicTacToe_Output();
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
