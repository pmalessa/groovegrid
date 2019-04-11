/*
 * DisguiseGame.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "DisguiseGame.h"

	DisguiseGame::DisguiseGame(GridTile *tile)
	{
		gameState = 0;
		lastTime = 0;
		flashTime = 0;
		this->tile = tile;
	}
	DisguiseGame::~DisguiseGame()
	{

	}
	void DisguiseGame::reset()
	{
		gameState = 0;
		lastTime = 0;
		flashTime = 0;
		fadePosition = 0;
	}
	void DisguiseGame::run()
	{
		uint16_t xPos, yPos;
		switch (gameState) {
			case 0:	//fade in
				if(Timer::getMillis()-lastTime > fadeDelay)
				{
					lastTime = Timer::getMillis();
					tile->drawLine(0, fadePosition, XMAX-1, fadePosition, tile->RGB(255, 0, 0));	//draw red line
					fadePosition++;
					if(fadePosition == YMAX)
					{
						flashTime = genFlashInterval();
						gameState = 1;
					}
				}

				break;
			case 1:	//flash sometimes
				if(Timer::getMillis()-lastTime > flashTime)
				{
					lastTime = Timer::getMillis();
					xPos = rand()%XMAX;
					yPos = rand()%YMAX;
					tile->drawPixel(xPos,yPos, tile->RGB(rand()%256, rand()%256, rand()%256));
					delay(flashDuration);
					tile->drawPixel(xPos,yPos, tile->RGB(255, 0, 0));
					flashTime = genFlashInterval();
				}
				break;
			default:
				break;
		}
	}
	void DisguiseGame::onInput(char *data)
	{
		UNUSED(data);//No Input necessary
	}

	char* DisguiseGame::exportAppState()
	{
		return 0;	//not yet
	}

	void DisguiseGame::importAppState(char* json)
	{
		UNUSED(json); //not yet
	}

	//return percentage of counter expired
	uint8_t DisguiseGame::getProgress()
	{
		uint32_t currentCounter = Timer::getMillis()-lastTime;
		float fraction = currentCounter/flashTime;
		return (uint8_t) fraction*100;	//return percentage converted to integer
	}

	uint32_t DisguiseGame::genFlashInterval()
	{
		uint32_t tmp;
		do{
			tmp = rand()%maxFlashInterval;
		}while(tmp < minFlashInterval);	//generate random number between min and max
		return tmp*1000;	//in milliseconds
	}
