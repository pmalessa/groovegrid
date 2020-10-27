/*
 * SpectrumAnimation.cpp
 *
 *  Created on: 30.04.2019
 *      Author: pmale
 */

#include "SpectrumAnimation.h"


SpectrumAnimation::SpectrumAnimation(GridTile *tile):GrooveAnimation(tile)
{
	frameTimer.setTimeStep(FRAMERATE_MS);
	barTimer.setTimeStep(FRAMERATE_MS*2);
	initBars();

	//static Microphone& mic = Microphone::getInstance();	//start Microphone
	//UNUSED(mic);
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
		static Microphone& mic = Microphone::getInstance();
		mic.getFFT(fftBuffer);
		for(uint8_t i=0;i<BAR_NR;i++)
		{
			double height = 6*log10(fftBuffer[i+1])+6;
			if(height > barArray[i].len) //attack
			{
				barArray[i].len = BAR_AT*height + (1-BAR_AT)*barArray[i].len;
			}
			else //release
			{
				barArray[i].len = (1-BAR_RT)*barArray[i].len;
			}
			if(barArray[i].len > tile->getHeight()-1)
			{
				barArray[i].len = tile->getHeight()-1;
			}
			if(barArray[i].len < 0)
			{
				barArray[i].len = 0;
			}
		}
	}
}

void SpectrumAnimation::drawBars()
{
	tile->fillScreenBuffer(CRGB(0));
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		tile->writeRect(barArray[i].xPos,(tile->getHeight()-1) - (uint8_t)barArray[i].len, BAR_WIDTH, tile->getHeight()-1, barArray[i].color);
	}
	tile->endWrite();
}

void SpectrumAnimation::initBars()
{
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		barArray[i].xPos = BAR_WIDTH*i;
		barArray[i].len = 0;
		barArray[i].color = (CRGBW)ColorFromPalette(lavaPalette,(255/BAR_NR)*i,0xFF,LINEARBLEND);
	}
}
