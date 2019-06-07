/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "ColorPaletteAnimation.h"

ColorPaletteAnimation::ColorPaletteAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

ColorPaletteAnimation::~ColorPaletteAnimation()
{

}

GrooveApp* ColorPaletteAnimation::new_instance(GridTile *tile)
{
	return new ColorPaletteAnimation(tile);
}

void ColorPaletteAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
	    ChangePalettePeriodically();

	    static uint8_t startIndex = 0;
	    startIndex = startIndex + 1; /* motion speed */

	    FillLEDsFromPaletteColors( startIndex);
	    tile->endWrite();
	}
}

void ColorPaletteAnimation::FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for(int i=0;i<tile->getHeight();i++)
    {
    	for(int j=0;j<tile->getWidth();j++)
    	{
    		tile->writePixel(j, i, tile->RGB(ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending)));
    		colorIndex += 3;
    	}
    }
}

void ColorPaletteAnimation::ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 30;
    static uint8_t lastSecond = 99;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        //if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        //if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        //if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        //if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        //if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        //if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}
