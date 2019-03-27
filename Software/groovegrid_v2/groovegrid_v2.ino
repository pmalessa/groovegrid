
#include "2048/Game_2048.h"
#include "PLATFORM.h"
#include "driver/BUTTON.h"
#include "driver/COMM.h"
#include "utils/TaskScheduler.h"

#include "Animation/ANIMATION.h"
#include "driver/Grid.h"
#include "driver/Timer.h"
#include "TicTacToe/Game_TicTacToe.h"

uint8_t programstate = 0;
Game_2048 game_2048 = Game_2048();
uint32_t prevCounter = 0;

void timer()
{
	static uint16_t button_cnt = 0;
	switch (programstate) {
		case 0: //ANIMATION
			ANIMATION_vRunner();
			break;
		case 1:
			break;
		default:
			break;
	}
	button_cnt++;
	if(button_cnt > 10)	//every 100ms
	{
		button_cnt = 0;
		BUTTON_vRead();		//read Buttons
	}
}

void setup()
{
	BUTTON_vInit();
#if defined(__AVR__)
	srand(eeprom_read_word((uint16_t *)0x23));
	eeprom_update_word((uint16_t *)0x23, (uint16_t)rand());
#endif
	ANIMATION_vBoot();
	Timer::start();
}

// The loop function is called in an endless loop
void loop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();

	tsched.handleTasks();

	switch (programstate) {
		case 0:	//ANIMATION
			if(BUTTON_bIsPressed(BUTTON_UP) || comm.read() == '1')
			{
				programstate = 1;
				ANIMATION_vBoot();
				game_2048.reset();
				game_2048.start();
				tsched.Attach(&game_2048);
			}
			break;
		case 1:
			if(!game_2048.isRunning())
			{
				tsched.Detach(&game_2048);
				programstate = 0;//quit
			}
			break;
		default:
			break;
	}
	if(Timer::getMillis() > prevCounter)	//if 1ms passed
	{
		prevCounter = Timer::getMillis();
		timer();	//call Timer functions
	}
}
