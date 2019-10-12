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
    	if(i%2 == 0) //even row
    	{
        	for(int j=tile->getWidth()-1;j>=0;j--)
        	{
        		tile->writePixel(j, i, ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending));
        		colorIndex += 3;
        	}
    	}
    	else	//odd row
    	{
        	for(int j=0;j<tile->getWidth();j++)
        	{
        		tile->writePixel(j, i, ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending));
        		colorIndex += 3;
        	}
    	}
    }
}

void ColorPaletteAnimation::ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void ColorPaletteAnimation::SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

void ColorPaletteAnimation::SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

void ColorPaletteAnimation::SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
