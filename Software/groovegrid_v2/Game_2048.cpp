/*
 * Game_2048.cpp
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */
#include "Arduino.h"
#include "Game_2048.h"
#include "GameState.h"
#include "BUTTON.h"
#include "HardwareSerial.h"

GameState game = GameState();

bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = false;
bool moving = false;

void stopMoving() {
	movingUp = false;
	movingDown = false;
	movingLeft = false;
	movingRight = false;
	moving = false;
	game.fillRandomField();
	LED_vDrawBoard(game.board);
	LED_vShow();

}

void moveUp() {
	if (!moving && (game.canStep(UP) || game.canMerge(UP))) {
		movingUp = true;
		moving = true;
	}
}

void moveDown() {
	if (!moving && (game.canStep(DOWN) || game.canMerge(DOWN))) {
		movingDown = true;
		moving = true;
	}
}

void moveLeft() {
	if (!moving && (game.canStep(LEFT) || game.canMerge(LEFT))) {
		movingLeft = true;
		moving = true;
	}
}

void moveRight() {
	if (!moving && (game.canStep(RIGHT) || game.canMerge(RIGHT))) {
		movingRight = true;
		moving = true;
	}
}

void Game_2048_vSetup()
{
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
			moveUp();
			break;
		case 'd':
			moveDown();
			break;
		case 'r':
			moveRight();
			break;
		case 'l':
			moveLeft();
			break;
		case 'q':
			return false;
			break;
		default:
			break;
	}

	if(BUTTON_bIsPressed(BUTTON_UP))
		moveUp();
	if(BUTTON_bIsPressed(BUTTON_DOWN))
		moveDown();
	if(BUTTON_bIsPressed(BUTTON_LEFT))
		moveLeft();
	if(BUTTON_bIsPressed(BUTTON_RIGHT))
		moveRight();

	return true;	//keep running
}

void Game_2048_vSyncTask()	//every 1 ms
{
	static uint16_t game_cnt = 0;
	game_cnt++;
	if(game_cnt > 300)
	{
		game_cnt = 0;



		bool stopMovingAtEnd = false;
		if (moving) {
			if(movingUp)
			{
				if(game.move(UP)) {
					stopMovingAtEnd = true;
				}
			}
			else if(movingDown)
			{
				if(game.move(DOWN)) {
					stopMovingAtEnd = true;
				}
			}
			else if(movingLeft)
			{
				if(game.move(LEFT)) {
					stopMovingAtEnd = true;
				}
			}
			else if(movingRight)
			{
				if(game.move(RIGHT)) {
					stopMovingAtEnd = true;
				}
			}

			LED_vDrawBoard(game.board);
			LED_vShow();
		}

		if (stopMovingAtEnd)
			stopMoving();
	}
}


