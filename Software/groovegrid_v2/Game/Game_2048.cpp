/*
 * Game_2048.cpp
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */
#include "Game_2048.h"

#include "../driver/COMM.h"
#include "../driver/Grid.h"
#include "../driver/Timer.h"

#define GAME_SPEED 200	//lower is faster

GameState_2048 game = GameState_2048();
direction_t movdir = NONE;
bool running = false;
uint32_t previousMillisCounter = 0;

Game_2048::Game_2048()
{
	game.initializeBoard();
}

Game_2048::~Game_2048()
{
}

void Game_2048::start()
{
	running = true;
	DrawBoard(game.board);
}

void Game_2048::reset()
{
	game = GameState_2048();
	game.initializeBoard();
}

void Game_2048::onInput(char *data)
{
	switch (*data) {
		case 'u':
			move(UP);
			break;
		case 'd':
			move(DOWN);
			break;
		case 'r':
			move(RIGHT);
			break;
		case 'l':
			move(LEFT);
			break;
		default:
			break;
	}
}

char* Game_2048::exportAppState()
{
	return 0;	//not yet
}

void Game_2048::importAppState(char* json)
{
	UNUSED(json); //not yet
}

//for now: highest reached tile in percent, with 0=0% and 2048=100%
uint8_t Game_2048::getProgress()
{
	uint16_t tileValue = game.highestTile;
	uint8_t powerOfTwo = 0;
	do{
		tileValue = tileValue >> 1;	//div2
		powerOfTwo++;
	}while(tileValue > 0);
	return (uint8_t)(powerOfTwo/11)*100;	//2^11 is 2048
}


void Game_2048::move(direction_t dir) {
	if(movdir == NONE)
	{
		movdir = dir;
	}
}

void Game_2048::run()
{
	static uint8_t move_possible = 0;
	if((Timer::getMillis()-previousMillisCounter) >= GAME_SPEED)
	{
		previousMillisCounter = Timer::getMillis();
		if (movdir != NONE)	//moving
		{
			if(game.canStep(movdir) || game.canMerge(movdir))	//if move possible
			{
				move_possible = 1;
			}
			if(game.move(movdir))	//if move finished
			{
				movdir = NONE;

				if(move_possible > 0)	//if it was moving, spawn new field
				{
					game.fillRandomField();
				}
				move_possible = 0;
			}
			DrawBoard(game.board);
		}
	}
}

void Game_2048::DrawBoard(uint16_t arr[YMAX][XMAX])
{
	static Grid& grid = Grid::getInstance();

    for (uint8_t i = 0; i < YMAX; i++)
      for (uint8_t j = 0; j < XMAX; j++)
    	  DrawTile(j, i, arr[i][j]);
    grid.endWrite();
}

void Game_2048::DrawTile(uint16_t x, uint16_t y, uint16_t number)
{
	static Grid& grid = Grid::getInstance();
	uint16_t col = 0;
	switch (number) {
		case 2:
			 col = grid.RGB(COLOR_RED);
			break;
		case 4:
			col = grid.RGB(COLOR_GREEN);
			break;
		case 8:
			col = grid.RGB(COLOR_BLUE);
			break;
		case 16:
			col = grid.RGB(COLOR_WHITE);
			break;
		case 32:
			col = grid.RGB(COLOR_YELLOW);
			break;
		case 64:
			col = grid.RGB(COLOR_VIOLET);
			break;
		case 128:
			col = grid.RGB(COLOR_CYAN);
			break;
		case 256:
			col = grid.RGB(COLOR_PINK);
			break;
		case 512:
			col = grid.RGB(COLOR_ORANGE);
			break;
		case 1024:
			col = grid.RGB(COLOR_LIGHTGREEN);
			break;
		case 2048:
			col = grid.RGB(COLOR_PINKRED);
			break;
		default:
			break;
	}
	grid.writePixel(x, y, col);
	if(number > game.highestTile)	//update game progress
	{
		game.highestTile = number;
	}
}

GameState_2048::GameState_2048()
{
}

void GameState_2048::fillBoard(uint16_t value) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = 0; x < BOARD_WIDTH; ++x) {
			board[y][x] = value;
		}
	}
}

uint16_t GameState_2048::getFreeSpaces(){
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

void GameState_2048::fillRandomField() {
	uint8_t x,y;
	if(getFreeSpaces()!= 0)
	{
		do{
			x = rand() % BOARD_WIDTH;
			y = rand() % BOARD_HEIGHT;
		}while(board[y][x]!=0);
		if((rand() % 10) > 0)	//90% of cases
		{
			board[y][x] = 2;
		}
		else
		{
			board[y][x] = 4;	//10% of cases
		}
	}
}

void GameState_2048::initializeBoard() {
	fillBoard(0);

	fillRandomField();
	fillRandomField();
}

uint16_t GameState_2048::getField(uint8_t y, uint8_t x, direction_t direction)
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

void GameState_2048::setField(uint8_t y, uint8_t x, direction_t direction, uint16_t value)
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
 * Moves tiles one step to the direction on each call.
 *
 * Returns true if move is finished after the current step. Returns false otherwise.
 * If the tiles can still be moved or merged left, the move is unfinished.
 * It is only finished after all tiles are as far left as possible and merged.
 */

bool GameState_2048::move(direction_t direction)
{
	if(canStep(direction)) {
		step(direction);
		return false;
	} else {
		merge(direction);
		return true;
	}
}


void GameState_2048::step(direction_t direction) {
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

bool GameState_2048::canStep(direction_t direction) {
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

bool GameState_2048::canMerge(direction_t direction){
	for (uint8_t y = 0; y < BOARD_HEIGHT; ++y) {
		for (uint8_t x = 1; x < BOARD_WIDTH; ++x) {
			if (getField(y, x, direction) == getField(y, x-1, direction) && getField(y, x, direction) != 0) {
				return true;
			}
		}
	}
	return false;
}

void GameState_2048::merge(direction_t direction) {
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

void GameState_2048::removeInBetweenZeros(direction_t direction) {
	while(canStep(direction)) {
		step(direction);
	}
}
