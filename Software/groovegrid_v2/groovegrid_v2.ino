
#include "PLATFORM.h"
#include "driver/BUTTON.h"
#include "driver/COMM.h"

#include "Animation/ANIMATION.h"
#include "2048/Game_2048.h"
#include "driver/Grid.h"
#include "driver/Timer.h"
#include "TicTacToe/Game_TicTacToe.h"

uint8_t programstate = 0;
Game_2048 game_2048 = Game_2048();

class TimerCall : public Listener
{
public:
	TimerCall(){}
    void Update()
    {
    	static uint16_t button_cnt = 0;
    	switch (programstate) {
    		case 0: //ANIMATION
    			ANIMATION_vRunner();
    			break;
    		case 1:
    			game_2048.SyncTask();
    			break;
    		case 2:
    			Game_TicTacToe_Output();
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
};

void setup()
{
	static Timer& timer = Timer::getInstance();
	TimerCall timercall;
	timer.Attach(&timercall);

	BUTTON_vInit();
#if defined(__AVR__)
	srand(eeprom_read_word((uint16_t *)0x23));
	eeprom_update_word((uint16_t *)0x23, (uint16_t)rand());
#endif
	ANIMATION_vBoot();
}

// The loop function is called in an endless loop
void loop()
{
	static COMM& comm = COMM::getInstance();
	switch (programstate) {
		case 0:	//ANIMATION
			if(BUTTON_bIsPressed(BUTTON_UP) || comm.read() == '1')
			{
				programstate = 1;
				ANIMATION_vBoot();
				game_2048.Reset();
				game_2048.Start();
			}
			break;
		case 1:
			if(!game_2048.Loop())
			{
				programstate = 0;//quit
			}
			break;
		default:
			break;
	}
}
