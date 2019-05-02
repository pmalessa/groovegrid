/*
 * DisguiseGame.cpp
 *
 *  Created on: 07.04.2019
 *      Author: pmale
 */

#include "DisguiseGame.h"

	DisguiseGame::DisguiseGame(GridTile *tile):GrooveGame(tile)
	{
		frameTimer.setTimeStep(1000/tile->getWidth());
	}
	DisguiseGame::~DisguiseGame()
	{

	}

	GrooveApp* DisguiseGame::new_instance(GridTile *tile)
	{
		return new DisguiseGame(tile);
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
				if(frameTimer.isTimeUp())
				{
					tile->drawLine(tile->getWidth()-1-fadePosition, 0, tile->getWidth()-1-fadePosition, tile->getHeight()-1, tile->RGB(255, 0, 0));	//draw red line
					fadePosition++;
					if(fadePosition == tile->getWidth())
					{
						frameTimer.setTimeStep(genFlashInterval());
						gameState = 1;
					}
				}

				break;
			case 1:	//flash sometimes
				if(frameTimer.isTimeUp())
				{
					xPos = esp_random()%tile->getWidth();
					yPos = esp_random()%tile->getHeight();
					tile->drawPixel(xPos,yPos, tile->RGB(esp_random()%256, esp_random()%256, esp_random()%256));
					delay(flashDuration);
					tile->drawPixel(xPos,yPos, tile->RGB(255, 0, 0));
					frameTimer.setTimeStep(genFlashInterval());
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
