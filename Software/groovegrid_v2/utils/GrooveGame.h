/*
 * GrooveGame.h
 *
 *  Created on: 11.04.2019
 *      Author: pmale
 */

#ifndef UTILS_GROOVEGAME_H_
#define UTILS_GROOVEGAME_H_

#include "GrooveApp.h"

class GrooveGame : public GrooveApp, public CommInterface
{
public:
	GrooveGame(GridTile *tile):GrooveApp(tile){}
    virtual std::string onUserRead(uint8_t channelID) = 0;
    virtual void onUserWrite(std::string data, uint8_t channelID) = 0;
};



#endif /* UTILS_GROOVEGAME_H_ */
