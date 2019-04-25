/*
 * Game_2048.h
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */

#ifndef GAME_2048_H_
#define GAME_2048_H_

#include "../PLATFORM.h"
#include "../utils/GrooveGame.h"
#include "../driver/COMM.h"
#include "../driver/Timer.h"
#include "../driver/GridTile.h"

#define BOARD_HEIGHT YMAX
#define BOARD_WIDTH XMAX

enum direction_t{
	NONE,
	LEFT,
	DOWN,
	RIGHT,
	UP
};

typedef enum{
	COLOR_RED = 0xFF0000,
	COLOR_ORANGE = 0xFF8000,
	COLOR_YELLOW = 0xFFFF00,
	COLOR_LIGHTGREEN = 0x80FF00,
	COLOR_GREEN = 0x00FF00,
	COLOR_BLUEGREEN = 0x00FF80,
	COLOR_CYAN = 0x00FFFF,
	COLOR_LIGHTBLUE = 0x0080FF,
	COLOR_BLUE = 0x0000FF,
	COLOR_VIOLET = 0x8000FF,
	COLOR_PINK = 0xFF00FF,
	COLOR_PINKRED = 0xFF0080,
	COLOR_BLACK = 0x000000,
	COLOR_WHITE = 0xFFFFFF
}color_t;

class Game_2048 : public GrooveGame
{
public:
	Game_2048(GridTile *tile);
	~Game_2048();
	void start();
	void pause();
	void stop();
	void run();
    std::string onUserRead(uint8_t channelID);
    void onUserWrite(std::string data, uint8_t channelID);
private:
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
	uint16_t highestTile;

	private:
		void merge(direction_t direction);
		void step(direction_t direction);
		void removeInBetweenZeros(direction_t direction);
};

#endif /* GAME_2048_H_ */
