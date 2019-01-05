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
};


