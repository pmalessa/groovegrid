/*
 * Game_TicTacToe.c
 *
 *  Created on: Jan 19, 2019
 *      Author: Kai
 */

#include "Arduino.h"

#include "../driver/GRID.h"
#include "Game_TicTacToe.h"

static uint8_t GameBoard[][4] = {{0,1,1,2},{1,0,2,0},{1,2,0,1},{1,2,2,0}};
static uint8_t playerPosition[] = {1,1};
static uint8_t playerTurn = 2;



void Game_TicTacToe_Output(){

	static uint16_t game_cnt = 0;
	static uint8_t blink_cnt = 0;
	// draw complete board
	for(int i = 0; i < 16 ; i++){

		switch(GameBoard[i / xMax][ i % xMax]){
			case 1 :
				LED_vDrawPixel( i % xMax, i / xMax, LED_u16Color(0, 0, 255));
				break;

			case 2 :
				LED_vDrawPixel( i % xMax, i / xMax, LED_u16Color(0, 255, 0));
				break;

			default :
				LED_vDrawPixel( i % xMax, i / xMax, LED_u16Color(0, 0, 0));
				break;
		}
	}

	game_cnt++;
	if(game_cnt > 1000){
		blink_cnt++;
		if(blink_cnt % 2 == 1){
			LED_vDrawPixel( playerPosition[0], playerPosition[1], LED_u16Color(255, 255, 255));
		}else{
			if(playerTurn == 1){
				LED_vDrawPixel( playerPosition[0], playerPosition[1], LED_u16Color(0, 0, 255));
			}else{
				LED_vDrawPixel( playerPosition[0], playerPosition[1], LED_u16Color(0, 255, 0));
			}
		}
		game_cnt = 0;
	LED_vShow();
	}
}

