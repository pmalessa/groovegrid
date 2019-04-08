/*
 * Transition_Boot.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "BootTransition.h"

BootTransition::BootTransition(GridTile* gridTile)
{
	tile = gridTile;
	running = false;
}
void BootTransition::run()
{
	tile->drawLine(0, 1, XMAX-1, 1, tile->RGB(255, 0, 0));
	tile->drawLine(0, 2, XMAX-1, 2, tile->RGB(255, 0, 0));
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
