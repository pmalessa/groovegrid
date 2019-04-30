/*
 * DisguiseGame.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "DisguiseGame.h"

	DisguiseGame::DisguiseGame(GridTile *tile):GrooveGame(tile)
	{
		gameState = 0;
		lastTime = 0;
		flashTime = 0;
	}
	DisguiseGame::~DisguiseGame()
	{

	}

	void DisguiseGame::start()
	{

	}

	void DisguiseGame::stop()
	{

	}

	void DisguiseGame::run()
	{
		uint16_t xPos, yPos;
		switch (gameState) {
			case 0:	//fade in
				if(Timer::getMillis()-lastTime > fadeDelay)
				{
					lastTime = Timer::getMillis();
					tile->drawLine(0, fadePosition, GRID_WIDTH-1, fadePosition, tile->RGB(255, 0, 0));	//draw red line
					fadePosition++;
					if(fadePosition == GRID_HEIGHT)
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
					xPos = esp_random()%GRID_WIDTH;
					yPos = esp_random()%GRID_HEIGHT;
					tile->drawPixel(xPos,yPos, tile->RGB(esp_random()%256, esp_random()%256, esp_random()%256));
					delay(flashDuration);
					tile->drawPixel(xPos,yPos, tile->RGB(255, 0, 0));
					flashTime = genFlashInterval();
				}
				break;
			default:
				break;
		}
	}
    std::string DisguiseGame::onUserRead(uint8_t channelID)
    {
    	UNUSED(channelID);
    	return "0";
    }
    void DisguiseGame::onUserWrite(std::string data, uint8_t channelID)
    {
    	UNUSED(data);
    	UNUSED(channelID);
    }

	uint32_t DisguiseGame::genFlashInterval()
	{
		uint32_t tmp;
		do{
			tmp = esp_random()%maxFlashInterval;
		}while(tmp < minFlashInterval);	//generate random number between min and max
		return tmp*1000;	//in milliseconds
	}
