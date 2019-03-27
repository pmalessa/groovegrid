/*
 * Game_2048.h
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */

#ifndef GAME_2048_H_
#define GAME_2048_H_

#include "../PLATFORM.h"
#include "../utils/Game.h"

#define BOARD_HEIGHT YMAX
#define BOARD_WIDTH XMAX

enum direction_t{
	NONE,
	LEFT,
	DOWN,
	RIGHT,
	UP
};

class Game_2048 : public Game
{
public:
	Game_2048();
	~Game_2048();
	void start();
	void stop();
	void reset();
	void run();

	bool isRunning();	//removed later, glue code
private:
	void SyncTask();
	void move(direction_t dir);
	void DrawBoard(uint16_t arr[YMAX][XMAX]);
	void DrawTile(uint16_t x, uint16_t y, uint16_t number);
};

class GameState_2048 {
public:
	GameState_2048();

	void fillBoard(uint16_t value);
	uint16_t getFreeSpaces();
	void fillRandomField();
	void initializeBoard();
	uint16_t getField(uint8_t y, uint8_t x, direction_t direction);
	void setField(uint8_t y, uint8_t x, direction_t direction, uint16_t value);
	bool move(direction_t direction);
	bool canStep(direction_t direction);
	bool canMerge(direction_t direction);
	uint16_t board[BOARD_HEIGHT][BOARD_WIDTH];

	private:
		void merge(direction_t direction);
		void step(direction_t direction);
		void removeInBetweenZeros(direction_t direction);
};

#endif /* GAME_2048_H_ */
