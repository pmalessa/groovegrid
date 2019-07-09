/*
 * Transition_Boot.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "BootTransition.h"

BootTransition::BootTransition(GridTile* gridTile):GrooveTransition(gridTile)
{
	running = false;
}
void BootTransition::run()
{
	tile->writeLine(0, 1, GRID_WIDTH-1, 1, CRGB(255, 0, 0));
	tile->endWrite();
	tile->writeLine(0, 2, GRID_WIDTH-1, 2, CRGB(255, 0, 0));
	tile->endWrite();
/*
	for (uint8_t i=0; i < YMAX; i++) {
		tile->drawLine(0, i, XMAX-1, i, tile->RGB(255, 0, 0));
		delay(100);
	}
	for (uint8_t i=0; i < YMAX; i++) {
		tile->drawLine(0, i, XMAX-1, i, tile->RGB(0, 0, 0));
		delay(100);
	}
*/
	running = false;
}

