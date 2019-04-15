/*
 * GrooveGame.h
 *
 *  Created on: 11.04.2019
 *      Author: pmale
 */

#ifndef UTILS_GROOVEGAME_H_
#define UTILS_GROOVEGAME_H_

#include "GrooveApp.h"

class GrooveGame : public GrooveApp
{
public:
	GrooveGame(GridTile *tile):GrooveApp(tile){}
	virtual char* exportAppState() = 0;
	virtual void importAppState(char *json) = 0;
	virtual uint8_t getProgress() = 0;				//return Progress 0..100
	virtual void reset() = 0;
};



#endif /* UTILS_GROOVEGAME_H_ */
