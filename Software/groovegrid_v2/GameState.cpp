/*
 * GameState.cpp
 *
 *  Created on: 05.01.2019
 *      Author: Chris
 */
#include "Arduino.h"

#include "GameState.h"
#include "LED.h"

//const uint8_t BOARD_HEIGHT = YMAX;
//const uint8_t BOARD_WIDTH = XMAX;
//const uint8_t BOARD_SIZE = BOARD_HEIGHT * BOARD_WIDTH;







//uint16_t board[BOARD_HEIGHT][BOARD_WIDTH];

GameState::GameState()
{
	//initializeBoard();
	initializeTestBoard();
}

void GameState::fillBoard(uint16_t value) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
			board[y][x] = value;
		}
	}
}

void GameState::fillRandomField() {

}


void GameState::initializeBoard() {
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

void GameState::initializeTestBoard() {
	board[1][0] = 2;
	board[2][0] = 8;
	board[2][3] = 2;
	board[1][3] = 2;
	board[2][1] = 2;

	board[0][0] = 2;
	board[0][1] = 2;
	board[0][2] = 2;
	board[0][3] = 2;
}

/*
 * Moves tiles one step to the left on each call.
 *
 * Returns true if move is finished after the current step. Returns false otherwise.
 * If the tiles can still be moved or merged left, the move is unfinished.
 * It is only finished after all tiles are as far left as possible and merged.
 */
bool GameState::moveLeft() {
	if(canStepLeft()) {
		stepLeft();
		return false;
	} else {
		mergeLeft();
		return true;
	}
}

void GameState::stepLeft() {
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

bool GameState::canStepLeft() {
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

void GameState::mergeLeft() {
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

void GameState::removeInBetweenZerosLeft() {
	while(canStepLeft()) {
		stepLeft();
	}
}



