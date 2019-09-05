/*
 * Game_2048.h
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */

#ifndef GAME_SNAKE
#define GAME_SNAKE

#include "../PLATFORM.h"
#include "../utils/GrooveGame.h"
#include "../driver/Timer.h"
#include "../driver/GridTile.h"
#include <deque>
#include "../driver/BluetoothService.h"

struct Coordinate{
	int8_t x;
	int8_t y;
	Coordinate(int8_t x, int8_t y);
};


class SnakeGame : public GrooveGame
{
public:
	SnakeGame(GridTile *tile);
	~SnakeGame();
	void start();
	void stop();
	void run();
	void onCommand(DynamicJsonDocument doc, uint8_t userID);
    GrooveApp* new_instance(GridTile *tile);

    uint32_t color = 0xFF0000;
private:
    enum MovementDirection{
    	none,
    	left,
    	down,
    	right,
    	up
    };

    MovementDirection direction = none;

    uint32_t frameDelay = 5;

    void draw();
    void drawElement(uint8_t x, uint8_t y);
    void drawElement(Coordinate coordinate);
    Coordinate* food;
    bool detectCollision();
    void wrapAroundBorder();

    void spawnFood();

    void move();
    void initialize();

	class SnakeGameState {
	public:
		std::deque<Coordinate*> body;
		Coordinate* head;

	};

	SnakeGameState *gameState;
};



#endif /* GAME_2048_H_ */
