/*
 * SimplyRedAnimation.cpp
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#include "SimplyRedAnimation.h"

	SimplyRedAnimation::SimplyRedAnimation(GridTile *tile):GrooveAnimation(tile)
	{
		frameTimer.setTimeStep(1000/tile->getWidth());
	}
	SimplyRedAnimation::~SimplyRedAnimation()
	{

	}

	GrooveApp* SimplyRedAnimation::new_instance(GridTile *tile)
	{
		return new SimplyRedAnimation(tile);
	}

	void SimplyRedAnimation::start()
	{

	}

	void SimplyRedAnimation::stop()
	{

	}

	void SimplyRedAnimation::run()
	{
		uint16_t xPos, yPos;
		switch (gameState) {
			case 0:	//fade in
				if(frameTimer.isTimeUp())
				{
					tile->writeLine(tile->getWidth()-1-fadePosition, 0, tile->getWidth()-1-fadePosition, tile->getHeight()-1, CRGB(255, 0, 0));	//draw red line
					tile->endWrite();
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
					tile->writePixel(xPos,yPos, CRGB(esp_random()%256, esp_random()%256, esp_random()%256));
					tile->endWrite();
					delay(flashDuration);
					tile->writePixel(xPos,yPos, CRGB(255, 0, 0));
					tile->endWrite();
					frameTimer.setTimeStep(genFlashInterval());
				}
				break;
			default:
				break;
		}
	}

	uint32_t SimplyRedAnimation::genFlashInterval()
	{
		uint32_t tmp;
		do{
			tmp = esp_random()%maxFlashInterval;
		}while(tmp < minFlashInterval);	//generate random number between min and max
		return tmp*1000;	//in milliseconds
	}


