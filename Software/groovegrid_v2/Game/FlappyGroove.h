/*
 * FlappyGroove.h
 *
 *  Created on: 28.04.2019
 *      Author: pmale
 */

#ifndef GAME_FLAPPYGROOVE_H_
#define GAME_FLAPPYGROOVE_H_

#include "../PLATFORM.h"
#include "../utils/GrooveGame.h"

class FlappyGroove : public GrooveGame{
public:
	FlappyGroove(GridTile *tile);
	~FlappyGroove();
	void run();
	void start();
	void stop();
    std::string onUserRead(uint8_t channelID);
    void onUserWrite(std::string data, uint8_t channelID);
private:
    uint16_t gameSpeed;


    class FlappyGrooveState
    {
    public:
    	uint8_t **board;
    	uint16_t xmax, ymax;
    };
    FlappyGrooveState *gameState;
};



#endif /* GAME_FLAPPYGROOVE_H_ */
