/*
 * Game_2048.cpp
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */
#include "Arduino.h"
#include "Game_2048.h"
#include "BUTTON.h"
#include "HardwareSerial.h"

#include "GameState_2048.h"

GameState_2048 game = GameState_2048();
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
	LED_vDrawBoard(game.board);
	LED_vShow();
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

void Game_2048_vSyncTask()	//every 1 ms
{
	static uint16_t game_cnt = 0;
	static uint8_t move_possible = 0;
	game_cnt++;
	if(game_cnt > 300)
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
					LED_vDrawBoard(game.board);
					LED_vShow();
				}
				move_possible = 0;
			}
			LED_vDrawBoard(game.board);
			LED_vShow();
		}
	}
}


