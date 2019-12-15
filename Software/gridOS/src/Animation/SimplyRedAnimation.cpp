/*
 * SimplyRedAnimation.cpp
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#include "SimplyRedAnimation.h"

	SimplyRedAnimation::SimplyRedAnimation(GridTile *tile):GrooveAnimation(tile)
	{
		fadeTimer.setTimeStep(1000/tile->getWidth());
	}
	SimplyRedAnimation::~SimplyRedAnimation()
	{

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
				if(fadeTimer.isTimeUp())
				{
					tile->writeLine(tile->getWidth()-1-fadePosition, 0, tile->getWidth()-1-fadePosition, tile->getHeight()-1, CRGB(255, 0, 0));	//draw red line
					tile->endWrite();
					fadePosition++;
					if(fadePosition == tile->getWidth())
					{
						gameState = 1;
					}
				}
				break;
			case 1:	//stay red
				if(frameTimer.isTimeUp())
				{
					tile->writeFilledRect(0,0,tile->getWidth()-1,tile->getHeight()-1,CRGB(255, 0, 0));
					tile->endWrite();
				}
				break;
		}
	}


