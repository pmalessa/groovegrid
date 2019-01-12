/*
 * GameState.h
 *
 *  Created on: 05.01.2019
 *      Author: Chris
 */
#ifndef GameState_h
#define GameState_h
#include "Arduino.h"
#include "LED.h"

#define BOARD_HEIGHT YMAX
#define BOARD_WIDTH XMAX

enum direction_t{
	LEFT,
	DOWN,
	RIGHT,
	UP
};

class GameState {
public:
	GameState();

	void fillBoard(uint16_t value);
	uint16_t getFreeSpaces();
	void fillRandomField();
	void initializeBoard();
	uint16_t getField(uint8_t y, uint8_t x, direction_t direction);
	void setField(uint8_t y, uint8_t x, direction_t direction, uint8_t value);
	bool move(direction_t direction);
	bool canStep(direction_t direction);
	bool canMerge(direction_t direction);
	uint16_t board[BOARD_HEIGHT][BOARD_WIDTH];

	private:
		void merge(direction_t direction);
		void step(direction_t direction);
		void removeInBetweenZeros(direction_t direction);
};

#endif
