/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomRectAnimation.h"

RandomRectAnimation::RandomRectAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	currentRed = 0;
	currentGreen = 0;
	currentRed = 0;
	stepSize = 10;			//10 brightness steps per frame

	generateNewRect();
}

RandomRectAnimation::~RandomRectAnimation()
{
}

GrooveApp* RandomRectAnimation::new_instance(GridTile *tile)
{
	return new RandomRectAnimation(tile);
}

void RandomRectAnimation::run()
{
	//static uint8_t i = 0,j = 0, on = 1;
	if(frameTimer.isTimeUp())
	{
		/*
		if(on == 1)
		{
			tile->drawPixel(i, j, tile->RGB(100, 100, 100));
		}
		else
		{
			tile->drawPixel(i, j, tile->RGB(0, 255, 0));
		}

		if(i == tile->getWidth()-1)
		{
			i=0;
			if(j == tile->getHeight()-1)
			{
				j=0;
				if(on == 1)
				{
					on = 0;
				}
				else
				{
					on = 1;
				}
			}
			else
			{
				j++;
			}
		}
		else
		{
			i++;
		}
		*/
		if(fadeIn == true)	//clear or draw
		{
			if(currentRed + stepSize > desiredRed) 		currentRed = desiredRed; 		else currentRed += stepSize;
			if(currentGreen + stepSize > desiredGreen)	currentGreen = desiredGreen; 	else currentGreen += stepSize;
			if(currentBlue + stepSize > desiredBlue) 	currentBlue = desiredBlue; 		else currentBlue += stepSize;
			tile->writeFilledRect(x, y, w, h, CRGB(currentRed, currentGreen, currentBlue));
			tile->endWrite();
			if(currentRed == desiredRed && currentGreen == desiredGreen && currentBlue == desiredBlue)
			{
				fadeIn = false;
			}
		}
		else
		{
			if(currentRed-stepSize < 0) 	currentRed=0; 		else currentRed -= stepSize;
			if(currentGreen-stepSize < 0) 	currentGreen=0; 	else currentGreen -= stepSize;
			if(currentBlue-stepSize < 0) 	currentBlue=0; 		else currentBlue -= stepSize;
			tile->writeFilledRect(x, y, w, h, CRGB(currentRed, currentGreen, currentBlue));
			tile->endWrite();
			if(currentRed == 0 && currentGreen == 0 && currentBlue == 0)
			{
				generateNewRect();
			}
		}
	}
}

void RandomRectAnimation::generateNewRect()
{
	fadeIn = true;
	desiredRed = esp_random()%256;
	desiredGreen = esp_random()%256;
	desiredRed = esp_random()%256;
	x = esp_random()%GRID_WIDTH;
	y = esp_random()%GRID_HEIGHT;
	w = esp_random()%GRID_WIDTH;
	h = esp_random()%GRID_HEIGHT;
}
