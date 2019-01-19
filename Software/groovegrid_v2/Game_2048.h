/*
 * Game_2048.h
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */

#ifndef GAME_2048_H_
#define GAME_2048_H_

#include "LED.h"

void Game_2048_vSetup();
/*
 * Keep running if returns true
 */
uint8_t Game_2048_u8Loop();
void Game_2048_vSyncTask();

void Game_2048_DrawBoard(uint16_t arr[YMAX][XMAX]);
void Game_2048_DrawTile(uint16_t x, uint16_t y, uint16_t number);

#endif /* GAME_2048_H_ */
