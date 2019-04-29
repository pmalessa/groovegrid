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
    void spawnWall();
    void moveWorld();
    void initBoard();
    void drawBoard();

    uint16_t gameSpeed;
    uint8_t maxWallHeight;
    uint8_t wallProbability;
    uint8_t worldMoveSpeed;

    enum boardField{
    	FIELD_EMPTY,		//empty field
		FIELD_WALL,			//top or bottom wall
		FIELD_SPACE,		//space between top and bottom wall
		FIELD_PLAYER		//Player Block
    };


    class FlappyGrooveState
    {
    public:
    	boardField **board;
    	uint16_t xmax, ymax;
    };
    FlappyGrooveState *gameState;
};



#endif /* GAME_FLAPPYGROOVE_H_ */
