/*
 * Game_2048.cpp
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */
#include "Game_2048.h"

Game_2048::Game_2048(GridTile *tile):GrooveGame(tile)
{
	gameSpeed = 5;
	tile->fillScreen(CRGB(0, 0, 0));	//fill grid black
	if(tile->getWidth() <= tile->getHeight())	//make board square
	{
		this->boardsize = tile->getWidth();
	}
	else
	{
		this->boardsize = tile->getHeight();
	}
	gameState = new GameState_2048(this->boardsize);
	gameState->initializeBoard();
}

Game_2048::~Game_2048()
{
	delete gameState;
}

GrooveApp* Game_2048::new_instance(GridTile *tile)
{
	return new Game_2048(tile);
}

void Game_2048::start()
{
	DrawBoard(gameState->board);
}

void Game_2048::stop()
{
}

void Game_2048::onCommand(CommandMsg *msg)
{
	std::string moveCmd = (*msg->doc)["move"].as<std::string>();

	if(moveCmd=="up")
	{
		move(UP);
	}
	else if(moveCmd=="down")
	{
		move(DOWN);
	}
	else if(moveCmd=="right")
	{
		move(RIGHT);
	}
	else if(moveCmd=="left")
	{
		move(LEFT);
	}
}

void Game_2048::load(DynamicJsonDocument *doc)
{
	boardsize = (*doc)["savegame"]["size"];	//this seems to work but is not very elegant...
	//boardsize = savegame["size"];
}
void Game_2048::save(DynamicJsonDocument *doc)
{
	JsonObject savegame = doc->createNestedObject("savegame");
	savegame["size"] = boardsize;
}

//for now: highest reached tile in percent, with 0=0% and 2048=100%
/*
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
*/


void Game_2048::move(direction_t dir) {
	if(movdir == NONE)
	{
		movdir = dir;
	}
}

void Game_2048::run()
{
	static uint16_t wait = 0;
	static uint8_t move_possible = 0;
	if(frameTimer.isTimeUp())
	{
		if(wait == 0)
		{
			wait = gameSpeed;
			if (movdir != NONE)	//moving
			{
				if(gameState->canStep(movdir) || gameState->canMerge(movdir))	//if move possible
				{
					move_possible = 1;
				}
				if(gameState->move(movdir))	//if move finished
				{
					movdir = NONE;

					if(move_possible > 0)	//if it was moving, spawn new field
					{
						gameState->fillRandomField();
					}
					move_possible = 0;
				}
				DrawBoard(gameState->board);
			}
		}
		else
		{
			wait--;
		}
	}
}

void Game_2048::DrawBoard(uint16_t **arr)
{
	static Grid& grid = Grid::getInstance();

    for (uint8_t i = 0; i < boardsize; i++)
      for (uint8_t j = 0; j < boardsize; j++)
    	  DrawTile(i, j, arr[i][j]);
    grid.endWrite();
}

void Game_2048::DrawTile(uint16_t x, uint16_t y, uint16_t number)
{
	uint16_t col = 0;
	switch (number) {
		case 2:
			 col = CRGB(COLOR_RED);
			break;
		case 4:
			col = CRGB(COLOR_GREEN);
			break;
		case 8:
			col = CRGB(COLOR_BLUE);
			break;
		case 16:
			col = CRGB(COLOR_WHITE);
			break;
		case 32:
			col = CRGB(COLOR_YELLOW);
			break;
		case 64:
			col = CRGB(COLOR_VIOLET);
			break;
		case 128:
			col = CRGB(COLOR_CYAN);
			break;
		case 256:
			col = CRGB(COLOR_PINK);
			break;
		case 512:
			col = CRGB(COLOR_ORANGE);
			break;
		case 1024:
			col = CRGB(COLOR_LIGHTGREEN);
			break;
		case 2048:
			col = CRGB(COLOR_PINKRED);
			break;
		default:
			break;
	}
	tile->writePixel(x, y, col);

	if(number > gameState->highestTile)	//update game progress
	{
		gameState->highestTile = number;
	}
}

GameState_2048::GameState_2048(uint8_t boardsize)
{
	this->boardsize = boardsize;
	highestTile = 0;

	board=new uint16_t*[boardsize];
	for(int i=0;i<boardsize;i++)
	{
		board[i]=new uint16_t[boardsize];
	}
}

GameState_2048::~GameState_2048()
{
	for(int i=0;i<boardsize;i++)	//delete board array
	{
		delete [] board[i];
	}
	delete [] board;

}

void GameState_2048::fillBoard(uint16_t value) {
	for (uint8_t x = 0; x < boardsize; ++x) {
		for (uint8_t y = 0; y < boardsize; ++y) {
			board[x][y] = value;
		}
	}
}

uint16_t GameState_2048::getFreeSpaces(){
	uint16_t cnt = 0;
	for (uint8_t x = 0; x < boardsize; ++x) {
		for (uint8_t y = 0; y < boardsize; ++y) {
			if(board[x][y] == 0)
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
			x = esp_random() % boardsize;
			y = esp_random() % boardsize;
		}while(board[x][y]!=0);
		if((esp_random() % 10) > 0)	//90% of cases
		{
			board[x][y] = 2;
		}
		else
		{
			board[x][y] = 4;	//10% of cases
		}
	}
}

void GameState_2048::initializeBoard() {
	fillBoard(0);

	fillRandomField();
	fillRandomField();
}

uint16_t GameState_2048::getField(uint8_t x, uint8_t y, direction_t direction)
{
	switch (direction) {
		case LEFT:
			return board[x][y];
			break;
		case UP:
			return board[boardsize-1-y][x];
			break;
		case RIGHT:
			return board[boardsize-1-x][boardsize-1-y];
			break;
		case DOWN:
			return board[y][boardsize-1-x];
			break;
		default:
			return 0;
			break;
	}
}

void GameState_2048::setField(uint8_t x, uint8_t y, direction_t direction, uint16_t value)
{
	switch (direction) {
		case LEFT:
			board[x][y] = value;
			break;
		case UP:
			board[boardsize-1-y][x] = value;
			break;
		case RIGHT:
			board[boardsize-1-x][boardsize-1-y] = value;
			break;
		case DOWN:
			board[y][boardsize-1-x] = value;
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


//default step is to the left
void GameState_2048::step(direction_t direction) {
	// For every line, start with the second tile from the left (the leftmost can't be pushed further)
	for (uint8_t x = 1; x < boardsize; ++ x) {
		for (uint8_t y = 0; y < boardsize; ++y) {
			// If tile to the left is zero, push tile left
			if (getField(x-1, y, direction) == 0) {
				setField(x-1, y, direction, getField(x, y, direction));
				setField(x, y, direction, 0);
			}
		}
	}
}

bool GameState_2048::canStep(direction_t direction) {
	for (uint8_t y = 0; y < boardsize; ++y) {
		bool zeroFound = false;
		for (uint8_t x = 0; x < boardsize; ++x) {
			if (zeroFound && getField(x, y, direction) != 0)
				return true;
			if (getField(x, y, direction) == 0)
				zeroFound = true;
		}
	}
	return false;
}

bool GameState_2048::canMerge(direction_t direction){
	for (uint8_t x = 1; x < boardsize; ++x) {
		for (uint8_t y = 0; y < boardsize; ++y) {
			if (getField(x, y, direction) == getField(x-1, y, direction) && getField(x, y, direction) != 0) {
				return true;
			}
		}
	}
	return false;
}

void GameState_2048::merge(direction_t direction) {
	for (uint8_t x = 1; x < boardsize; ++x) {
		for (uint8_t y = 0; y < boardsize; ++y) {
			if (getField(x, y, direction) == getField(x-1, y, direction)) {
				setField(x-1, y, direction, getField(x-1, y, direction) * 2);
				setField(x, y, direction, 0);
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