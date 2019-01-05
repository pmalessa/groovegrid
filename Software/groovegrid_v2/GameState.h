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

class GameState {
public:
	GameState();


	void fillBoard(uint16_t value);

	void fillRandomField();


	void initializeBoard();

		/*
		 * Moves tiles one step to the left on each call.
		 *
		 * Returns true if move is finished after the current step. Returns false otherwise.
		 * If the tiles can still be moved or merged left, the move is unfinished.
		 * It is only finished after all tiles are as far left as possible and merged.
		 */
	public:
		bool moveLeft();

	private:
		void stepLeft();

		bool canStepLeft();

		void mergeLeft();

		void removeInBetweenZerosLeft();

		void initializeTestBoard();

		uint16_t board[BOARD_HEIGHT][BOARD_WIDTH];
};

#endif
