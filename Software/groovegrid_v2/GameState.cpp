/*
 * GameState.cpp
 *
 *  Created on: 05.01.2019
 *      Author: Chris
 */
#include "Arduino.h"

#include "GameState.h"
#include "LED.h"

GameState::GameState()
{
	initializeBoard();
}

void GameState::fillBoard(uint16_t value) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
			board[y][x] = value;
		}
	}
}

uint16_t GameState::getFreeSpaces(){
	uint16_t cnt = 0;
	for(int i=0;i<BOARD_HEIGHT;i++)
	{
		for(int j=0;j<BOARD_WIDTH;j++)
		{
			if(board[i][j] == 0)
			{
				cnt++;
			}
		}
	}
	return cnt;
}

void GameState::fillRandomField() {
	uint8_t x,y;
	if(getFreeSpaces()!= 0)
	{
		do{
			x = rand() % BOARD_WIDTH;
			y = rand() % BOARD_HEIGHT;
		}while(board[y][x]!=0);
		board[y][x] = 2;
	}
}

void GameState::initializeBoard() {
	fillBoard(0);

	fillRandomField();
	fillRandomField();
}

uint16_t GameState::getField(uint8_t y, uint8_t x, direction_t direction)
{
	switch (direction) {
		case LEFT:
			return board[y][x];
			break;
		case DOWN:
			return board[BOARD_HEIGHT-1-x][y];
			break;
		case RIGHT:
			return board[BOARD_HEIGHT-1-y][BOARD_WIDTH-1-x];
			break;
		case UP:
			return board[x][BOARD_WIDTH-1-y];
			break;
		default:
			return 0;
			break;
	}
}

void GameState::setField(uint8_t y, uint8_t x, direction_t direction, uint8_t value)
{
	switch (direction) {
		case LEFT:
			board[y][x] = value;
			break;
		case DOWN:
			board[BOARD_HEIGHT-1-x][y]= value;
			break;
		case RIGHT:
			board[BOARD_HEIGHT-1-y][BOARD_WIDTH-1-x]= value;
			break;
		case UP:
			board[x][BOARD_WIDTH-1-y]= value;
			break;
		default:
			break;
	}
}

/*
 * Moves tiles one step to the left on each call.
 *
 * Returns true if move is finished after the current step. Returns false otherwise.
 * If the tiles can still be moved or merged left, the move is unfinished.
 * It is only finished after all tiles are as far left as possible and merged.
 */

bool GameState::move(direction_t direction)
{
	if(canStep(direction)) {
		step(direction);
		return false;
	} else {
		merge(direction);
		return true;
	}
}


void GameState::step(direction_t direction) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++ y) {
		// For every line, start with the second tile from the left (the leftmost can't be pushed further)
		for (uint8_t x = 1; x < BOARD_WIDTH; ++x) {
			// If tile to the left is zero, push tile left
			if (getField(y, x-1, direction) == 0) {
				setField(y, x-1, direction, getField(y, x, direction));
				setField(y, x, direction, 0);
			}
		}
	}
}

bool GameState::canStep(direction_t direction) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		bool zeroFound = false;
		for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
			if (zeroFound && getField(y, x, direction) != 0)
				return true;
			if (getField(y, x, direction) == 0)
				zeroFound = true;
		}
	}
	return false;
}

bool GameState::canMerge(direction_t direction){
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = 1; x < BOARD_WIDTH; ++x) {
			if (getField(y, x, direction) == getField(y, x-1, direction)) {
				return true;
			}
		}
	}
	return false;
}

void GameState::merge(direction_t direction) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = 1; x < BOARD_WIDTH; ++x) {
			if (getField(y, x, direction) == getField(y, x-1, direction)) {
				setField(y, x-1, direction, getField(y, x-1, direction) * 2);
				setField(y, x, direction, 0);
			}
		}
	}
	removeInBetweenZeros(direction);
}

void GameState::removeInBetweenZeros(direction_t direction) {
	while(canStep(direction)) {
		step(direction);
	}
}
