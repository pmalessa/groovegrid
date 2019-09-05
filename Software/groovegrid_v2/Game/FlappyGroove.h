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
	void onCommand(DynamicJsonDocument doc, uint8_t userID);
    GrooveApp* new_instance(GridTile *tile);
private:
    void spawnWall();
    void moveWorld();
    void initBoard();
    void drawBoard();
    void updatePlayer();
    bool isPlayerCollided();
    void restart();

    uint16_t gameSpeed;
    uint8_t maxWallHeight;
    uint8_t wallProbability;
    uint8_t worldMoveSpeed;
    uint8_t playerPosition;

    float velocity, gravity, position;

    enum boardField{
    	FIELD_EMPTY,		//empty field
		FIELD_WALL,			//top or bottom wall
		FIELD_SPACE,		//space between top and bottom wall
		FIELD_FLOOR
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
