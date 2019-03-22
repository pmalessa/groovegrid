/*
 * Game_2048.cpp
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */
#include "Game_2048.h"

#include "../driver/BUTTON.h"
#include "HardwareSerial.h"

#include "../driver/GRID.h"
#include "GameState_2048.h"

#define GAME_SPEED 200	//lower is faster

GameState_2048 game = GameState_2048();
Grid grid;
direction_t movdir = NONE;

void move(direction_t dir) {
	if(movdir == NONE)
	{
		movdir = dir;
	}
}

void Game_2048_vSetup()
{
	game.initializeBoard();
	Game_2048_DrawBoard(game.board);
	grid = grid.getInstance();
}

uint8_t Game_2048_u8Loop()
{
	int b = Serial.read();
	switch (b) {
		case -1:
			break;
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
		case 'q':
			return false;
			break;
		default:
			break;
	}

	if(BUTTON_bIsPressed(BUTTON_UP))
		move(UP);
	if(BUTTON_bIsPressed(BUTTON_DOWN))
		move(DOWN);
	if(BUTTON_bIsPressed(BUTTON_LEFT))
		move(LEFT);
	if(BUTTON_bIsPressed(BUTTON_RIGHT))
		move(RIGHT);

	return true;	//keep running
}

void Game_2048_DrawBoard(uint16_t arr[YMAX][XMAX])
{
    for (uint8_t i = 0; i < YMAX; i++)
      for (uint8_t j = 0; j < XMAX; j++)
    	  Game_2048_DrawTile(j, i, arr[i][j]);
    grid.endWrite();
}

void Game_2048_DrawTile(uint16_t x, uint16_t y, uint16_t number)
{
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
}

void Game_2048_vSyncTask()	//every 1 ms
{
	static uint16_t game_cnt = 0;
	static uint8_t move_possible = 0;
	game_cnt++;
	if(game_cnt > GAME_SPEED)
	{
		game_cnt = 0;

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
			Game_2048_DrawBoard(game.board);
		}
	}
}
