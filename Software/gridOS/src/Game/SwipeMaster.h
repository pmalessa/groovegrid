/*
 * GAME_SWIPEMASTER.h
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */

#ifndef GAME_SWIPEMASTER
#define GAME_SWIPEMASTER

#include "../PLATFORM.h"
#include "../utils/GrooveGame.h"
#include "../driver/BluetoothService.h"

class SwipeMaster : public GrooveGame
{
public:
	SwipeMaster(GridTile *tile);
	~SwipeMaster();
	void start();
	void stop();
	void run();
	void draw();
	void checkWin();
	void onCommand(CommandMsg *msg);

    uint32_t color = 0xFF0000;
private:
	void reset();
	int16_t position, middle;
};



#endif /* GAME_SWIPEMASTER */
