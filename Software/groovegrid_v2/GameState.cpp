/*
 * GameState.cpp
 *
 *  Created on: 05.01.2019
 *      Author: Chris
 */
#include "Arduino.h"

#include "GameState.h"
#include "LED.h"

const uint8_t BOARD_HEIGHT = YMAX;
const uint8_t BOARD_WIDTH = XMAX;
const uint8_t BOARD_SIZE = BOARD_HEIGHT * BOARD_WIDTH;





class GameState {

	uint16_t board[BOARD_HEIGHT][BOARD_WIDTH];

	GameState() {
		initializeBoard();
	}

	void fillBoard(uint16_t value) {
		for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
			for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
				board[y][x] = value;
			}
		}
	}

	void fillRandomField() {

	}


	void initializeBoard() {
		fillBoard(0);

		uint8_t firstTileX = rand() % BOARD_WIDTH;
		uint8_t firstTileY = rand() % BOARD_HEIGHT;

		uint8_t secondTileX = rand() % BOARD_WIDTH;
		uint8_t secondTileY = rand() % BOARD_HEIGHT;

		if (secondTileX == firstTileX && secondTileY == firstTileY)
			++secondTileX;

		board[firstTileY][firstTileX] = 2;
		board[secondTileY][secondTileX] = 2;


	}

	/*
	 * Moves tiles one step to the left on each call.
	 *
	 * Returns true if move is finished after the current step. Returns false otherwise.
	 * If the tiles can still be moved or merged left, the move is unfinished.
	 * It is only finished after all tiles are as far left as possible and merged.
	 */
	bool moveLeft() {
		if(canStepLeft()) {
			stepLeft();
			return false;
		} else {
			mergeLeft();
			return true;
		}
	}

private:
	void stepLeft() {
		for (uint8_t y = 0; y < BOARD_HEIGHT; ++ y) {
			// For every line, start with the second tile from the left (the leftmost can't be pushed further)
			for (uint8_t x = 1; x < BOARD_WIDTH; ++x) {
				// If tile to the left is zero, push tile left
				if (board[y][x-1] == 0) {
					board[y][x-1] = board[y][x];
					board[y][x] = 0;
				}
			}
		}
	}

	bool canStepLeft() {
		for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
			bool zeroFound = false;
			for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
				if (zeroFound && board[y][x] != 0)
					return true;
				if (board[y][x] == 0)
					zeroFound = true;
			}
		}
		return false;
	}

	void mergeLeft() {
		for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
			for (uint8_t x = 1; x < BOARD_WIDTH; ++x) {
				if (board[y][x] == board[y][x-1]) {
					board[y][x-1] = board[y][x-1] * 2;
					board[y][x] = 0;
				}
			}
		}
		removeInBetweenZerosLeft();
	}

	void removeInBetweenZerosLeft() {
		while(canStepLeft()) {
			stepLeft();
		}
	}
};


