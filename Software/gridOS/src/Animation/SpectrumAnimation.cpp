/*
 * SpectrumAnimation.cpp
 *
 *  Created on: 30.04.2019
 *      Author: pmale
 */

#include "SpectrumAnimation.h"
#include "../driver/Microphone.h"


SpectrumAnimation::SpectrumAnimation(GridTile *tile):GrooveAnimation(tile)
{
	frameTimer.setTimeStep(FRAMERATE_MS);
	barTimer.setTimeStep(FRAMERATE_MS*2);
	initBars();
}

void SpectrumAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		drawBars();
		updateBars();
	}
}

void SpectrumAnimation::updateBars()
{
	if(barTimer.isTimeUp())
	{
		for(uint8_t i=0;i<BAR_NR;i++)
		{
			if(barArray[i].len > 0)
			{
				barArray[i].len--;
			}
			else
			{
				barArray[i].len = esp_random()%tile->getHeight();
			}
		}
	}
}

void SpectrumAnimation::drawBars()
{
	tile->fillScreenBuffer(CRGB(0));
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		tile->writeRect(barArray[i].xPos,(tile->getHeight()-1) - barArray[i].len, BAR_WIDTH, tile->getHeight()-1, barArray[i].color);
	}
	tile->endWrite();
}

void SpectrumAnimation::initBars()
{
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		barArray[i].xPos = BAR_WIDTH*i;
		barArray[i].len = (tile->getHeight()/3) + esp_random()%((tile->getHeight()/3)*2);
		//barArray[i].color = CRGB(esp_random());
		barArray[i].color = ColorFromPalette(lavaPalette,(255/BAR_NR)*i,0xFF,LINEARBLEND);
	}
}
