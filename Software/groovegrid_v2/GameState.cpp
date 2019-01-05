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

const uint8_t N = BOARD_HEIGHT;





//uint16_t board[BOARD_HEIGHT][BOARD_WIDTH];

GameState::GameState()
{
	initializeBoard();
	//initializeTestBoard();
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
	for(int i=0;i<BOARD_HEIGHT-1;i++)
	{
		for(int j=0;j<BOARD_WIDTH-1;j++)
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


// ---------- RIGHT -------------


/*
 * Moves tiles one step to the right on each call.
 *
 * Returns true if move is finished after the current step. Returns false otherwise.
 * If the tiles can still be moved or merged right, the move is unfinished.
 * It is only finished after all tiles are as far right as possible and merged.
 */
bool GameState::moveRight() {
	if(canStepRight()) {
		stepRight();
		return false;
	} else {
		mergeRight();
		return true;
	}
}


void GameState::stepRight() {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++ y) {
		// For every line, start with the second tile from the left (the leftmost can't be pushed further)
		for (uint8_t x = BOARD_WIDTH - 1; x > 0; --x) {
			// If tile to the left is zero, push tile left
			if (board[y][x] == 0) {
				board[y][x] = board[y][x-1];
				board[y][x-1] = 0;
			}
		}
	}
}

bool GameState::canStepRight() {
	for (uint8_t y = BOARD_HEIGHT; y > 0; --y) {
		bool zeroFound = false;
		for (uint8_t x = BOARD_WIDTH; x > 0; --x) {
			if (zeroFound && board[y-1][x-1] != 0)
				return true;
			if (board[y-1][x-1] == 0)
				zeroFound = true;
		}
	}
	return false;
}

void GameState::mergeRight() {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = BOARD_WIDTH-1; x > 0; --x) {
			if (board[y][x-1] == board[y][x]) {
				board[y][x] = board[y][x] * 2;
				board[y][x-1] = 0;
			}
		}
	}
	removeInBetweenZerosRight();
}


void GameState::removeInBetweenZerosRight() {
	while(canStepRight()) {
		stepRight();
	}
}

// ------------- UP -------------

/*
 * Moves tiles one step to the left on each call.
 *
 * Returns true if move is finished after the current step. Returns false otherwise.
 * If the tiles can still be moved or merged left, the move is unfinished.
 * It is only finished after all tiles are as far left as possible and merged.
 */
bool GameState::moveUp() {
	if(canStepUp()) {
		stepUp();
		return false;
	} else {
		mergeUp();
		return true;
	}
}


void GameState::stepUp() {
	for (uint8_t x = 0; x < BOARD_HEIGHT; ++ x) {
		// For every line, start with the second tile from the left (the leftmost can't be pushed further)
		for (uint8_t y = 1; y < BOARD_WIDTH; ++y) {
			// If tile to the left is zero, push tile left
			if (board[y-1][x] == 0) {
				board[y-1][x] = board[y][x];
				board[y][x] = 0;
			}
		}
	}
}

bool GameState::canStepUp() {
	for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
		bool zeroFound = false;
		for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
			if (zeroFound && board[y][x] != 0)
				return true;
			if (board[y][x] == 0)
				zeroFound = true;
		}
	}
	return false;
}

void GameState::mergeUp() {
	for (uint8_t x = 0; x < BOARD_HEIGHT; ++x) {
		for (uint8_t y = 1; y < BOARD_WIDTH; ++y) {
			if (board[y][x] == board[y-1][x]) {
				board[y-1][x] = board[y-1][x] * 2;
				board[y][x] = 0;
			}
		}
	}
	removeInBetweenZerosUp();
}

void GameState::removeInBetweenZerosUp() {
	while(canStepUp()) {
		stepUp();
	}
}

// ---------- DOWN -------------


/*
 * Moves tiles one step to the right on each call.
 *
 * Returns true if move is finished after the current step. Returns false otherwise.
 * If the tiles can still be moved or merged right, the move is unfinished.
 * It is only finished after all tiles are as far right as possible and merged.
 */
bool GameState::moveDown() {
	if(canStepDown()) {
		stepDown();
		return false;
	} else {
		mergeDown();
		return true;
	}
}


void GameState::stepDown() {
	for (uint8_t x = 0; x < BOARD_HEIGHT; ++ x) {
		// For every line, start with the second tile from the left (the leftmost can't be pushed further)
		for (uint8_t y = BOARD_WIDTH - 1; y > 0; --y) {
			// If tile to the left is zero, push tile left
			if (board[y][x] == 0) {
				board[y][x] = board[y-1][x];
				board[y-1][x] = 0;
			}
		}
	}
}

bool GameState::canStepDown() {
	for (uint8_t x = BOARD_HEIGHT; x > 0; --x) {
		bool zeroFound = false;
		for (uint8_t y = BOARD_WIDTH; y > 0; --y) {
			if (zeroFound && board[y-1][x-1] != 0)
				return true;
			if (board[y-1][x-1] == 0)
				zeroFound = true;
		}
	}
	return false;
}

void GameState::mergeDown() {
	for (uint8_t x = 0; x < BOARD_HEIGHT; ++x) {
		for (uint8_t y = BOARD_WIDTH-1; y > 0; --y) {
			if (board[y-1][x] == board[y][x]) {
				board[y][x] = board[y][x] * 2;
				board[y-1][x] = 0;
			}
		}
	}
	removeInBetweenZerosDown();
}


void GameState::removeInBetweenZerosDown() {
	while(canStepDown()) {
		stepDown();
	}
}



// Helper Functions

// An Inplace function to rotate a N x N matrix
// by 90 degrees in anti-clockwise direction
void rotateMatrix(int mat[][N])
{
    // Consider all squares one by one
    for (int x = 0; x < N / 2; x++)
    {
        // Consider elements in group of 4 in
        // current square
        for (int y = x; y < N-x-1; y++)
        {
            // store current cell in temp variable
            int temp = mat[x][y];

            // move values from right to top
            mat[x][y] = mat[y][N-1-x];

            // move values from bottom to right
            mat[y][N-1-x] = mat[N-1-x][N-1-y];

            // move values from left to bottom
            mat[N-1-x][N-1-y] = mat[N-1-y][x];

            // assign temp to left
            mat[N-1-y][x] = temp;
        }
    }
}

