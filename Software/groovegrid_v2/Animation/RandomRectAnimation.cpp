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

void RandomRectAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		if(fadeIn == true)	//clear or draw
		{
			tile->fillRect(x, y, w, h, tile->RGB(currentRed, currentGreen, currentBlue));
			if(currentRed + stepSize > desiredRed) 		currentRed = desiredRed; 		else currentRed += stepSize;
			if(currentGreen + stepSize > desiredGreen)	currentGreen = desiredGreen; 	else currentGreen += stepSize;
			if(currentBlue + stepSize > desiredBlue) 	currentBlue = desiredBlue; 		else currentBlue += stepSize;
			if(currentRed == desiredRed && currentGreen == desiredGreen && currentBlue == desiredBlue)
			{
				fadeIn = false;
			}
		}
		else
		{
			tile->fillRect(x, y, w, h, tile->RGB(currentRed, currentGreen, currentBlue));
			if(currentRed-stepSize < 0) 	currentRed=0; 		else currentRed -= stepSize;
			if(currentGreen-stepSize < 0) 	currentGreen=0; 	else currentGreen -= stepSize;
			if(currentBlue-stepSize < 0) 	currentBlue=0; 		else currentBlue -= stepSize;
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
	desiredRed = rand()%256;
	desiredGreen = rand()%256;
	desiredRed = rand()%256;
	x = rand()%XMAX;
	y = rand()%YMAX;
	w = rand()%XMAX;
	h = rand()%YMAX;
}
