/*
 * SpectrumAnimation.cpp
 *
 *  Created on: 30.04.2019
 *      Author: pmale
 */

#include "FakeSpectrumAnimation.h"


FakeSpectrumAnimation::FakeSpectrumAnimation(GridTile *tile):GrooveAnimation(tile)
{
	frameTimer.setTimeStep(FRAMERATE_MS);
	barTimer.setTimeStep(FRAMERATE_MS*2);
	initBars();

	//static Microphone& mic = Microphone::getInstance();	//start Microphone
	//UNUSED(mic);
}

void FakeSpectrumAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		drawBars();
		updateBars();
	}
}

void FakeSpectrumAnimation::updateBars()
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
	/*
	static uint8_t maxHeight = 0;
	static Microphone& mic = Microphone::getInstance();
	mic.getFFT(fftBuffer);
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		barArray[i].len = (uint8_t) (fftBuffer[i+1]*tile->getHeight());	//avoid DC part
		if(barArray[i].len > tile->getHeight()-1)
		{
			barArray[i].len = tile->getHeight()-1;
		}
	}
	*/
}

void FakeSpectrumAnimation::drawBars()
{
	tile->fillScreenBuffer(CRGB(0));
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		tile->writeRect(barArray[i].xPos,(tile->getHeight()-1) - barArray[i].len, BAR_WIDTH, tile->getHeight()-1, barArray[i].color);
	}
	tile->endWrite();
}

void FakeSpectrumAnimation::initBars()
{
	for(uint8_t i=0;i<BAR_NR;i++)
	{
		barArray[i].xPos = BAR_WIDTH*i;
		barArray[i].len = 0;
		barArray[i].color = ColorFromPalette(lavaPalette,(255/BAR_NR)*i,0xFF,LINEARBLEND);
	}
}
