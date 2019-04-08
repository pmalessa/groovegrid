/*
 * DisguiseGame.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "DisguiseGame.h"

	DisguiseGame::DisguiseGame()
	{

	}
	DisguiseGame::~DisguiseGame()
	{

	}
	void DisguiseGame::reset()
	{

	}
	void DisguiseGame::run()
	{
		switch (gameState) {
			case 0:	//fade in


				break;
			case 1:	//flash sometimes

				break;
			default:
				break;
		}
	}
	void DisguiseGame::onInput(char *data)
	{
		//No Input necessary
	}

	uint32_t DisguiseGame::genFlashInterval()
	{
		uint32_t tmp = rand()%maxFlashInterval;
	}
