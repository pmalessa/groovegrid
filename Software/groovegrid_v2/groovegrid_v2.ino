
#include "PLATFORM.h"

#include "driver/BUTTON.h"
#include "driver/COMM.h"
#include "driver/GridTile.h"
#include "driver/Timer.h"
#include "utils/TaskScheduler.h"

#include "2048/Game_2048.h"
#include "Animation/AnimationRunner.h"

uint8_t programstate = 0;
Game_2048 game_2048 = Game_2048();
uint32_t prevCounter = 0;
uint8_t input = 0;

GridTile gridTile = GridTile(0, 0, 3, 3);
AnimationRunner animationRunner = AnimationRunner(&gridTile);

class MainListener : public InputListener
{
	void onInput(char *data)
	{
		input = *data;
	}
};

void setup()
{

	static COMM& comm = COMM::getInstance();
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static MainListener mainlistener;

#if defined(__AVR__)
	srand(eeprom_read_word((uint16_t *)0x23));
	eeprom_update_word((uint16_t *)0x23, (uint16_t)rand());
#endif

	Timer::start();
	tsched.Attach(&comm);
	comm.Attach(&mainlistener, COMM::MAIN);
	tsched.Attach(&animationRunner);
	animationRunner.start();
}

// The loop function is called in an endless loop
void loop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	static COMM& comm = COMM::getInstance();
	UNUSED(comm);

	tsched.handleTasks();

	switch (programstate) {
		case 0:	//ANIMATION

			if(input == '1')
			{
				animationRunner.stop();
				tsched.Detach(&animationRunner);
				input = 0;
				programstate = 1;
				//ANIMATION_vBoot();
				game_2048.start();
				comm.Attach(&game_2048, COMM::APP);	//attach input to app
				tsched.Attach(&game_2048);
			}
			break;
		case 1:
			if(input == 'q')
			{
				input = 0;
				tsched.Detach(&game_2048);
				comm.Detach(&game_2048, COMM::APP);	//detach input to app
				tsched.Attach(&animationRunner);
				animationRunner.start();
				programstate = 0;//quit
			}
			if(input == 'x')
			{
				input = 0;
				//ANIMATION_vBoot();
				game_2048.reset();
				game_2048.start();
			}
			break;
		default:
			break;
	}

}
