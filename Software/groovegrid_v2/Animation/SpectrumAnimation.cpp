/*
 * SpectrumAnimation.cpp
 *
 *  Created on: 30.04.2019
 *      Author: pmale
 */

#include "SpectrumAnimation.h"

SpectrumAnimation::SpectrumAnimation(GridTile *tile):GrooveAnimation(tile)
{
	frameTimer.setTimeStep(FRAMERATE_MS*15);
	barWidth = 3;
	barNumber = tile->getWidth()/barWidth;
	barArray = new Bar[barNumber];	//3 wide Bars
	generateBars();
}

GrooveApp* SpectrumAnimation::new_instance(GridTile *tile)
{
	return new SpectrumAnimation(tile);
}

void SpectrumAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		drawBars();
		generateBars();
	}
}

void SpectrumAnimation::drawBars()
{
	for(uint8_t i=0;i<barNumber;i++)
	{
		tile->writeRect(barArray[i].xPos, barArray[i].yPos, barWidth, tile->getHeight()-1, barArray->color);
	}
	tile->endWrite();
}

void SpectrumAnimation::generateBars()
{
	for(uint8_t i=0;i<barNumber;i++)
	{
		barArray[i].xPos = barWidth*i;
		barArray[i].yPos = esp_random()%tile->getHeight();
		barArray[i].color = CRGB(esp_random());
	}
}
