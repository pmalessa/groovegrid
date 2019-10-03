/*
 * ColorPaletteAnimation.h
 *
 *  Created on: 01.06.2019
 *      Author: pmale
 */

#ifndef ANIMATION_COLORPALETTEANIMATION_H_
#define ANIMATION_COLORPALETTEANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class ColorPaletteAnimation: public GrooveAnimation
{
public:
	ColorPaletteAnimation(GridTile* gridTile);
	~ColorPaletteAnimation();
	void run();
private:
	void FillLEDsFromPaletteColors( uint8_t colorIndex);
	void ChangePalettePeriodically();

	void SetupTotallyRandomPalette();
	void SetupBlackAndWhiteStripedPalette();
	void SetupPurpleAndGreenPalette();
	CRGBPalette16 currentPalette;
	TBlendType    currentBlending;
	const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
	{
	    CRGB::Red,
	    CRGB::Gray, // 'white' is too bright compared to red and blue
	    CRGB::Blue,
	    CRGB::Black,

	    CRGB::Red,
	    CRGB::Gray,
	    CRGB::Blue,
	    CRGB::Black,

	    CRGB::Red,
	    CRGB::Red,
	    CRGB::Gray,
	    CRGB::Gray,
	    CRGB::Blue,
	    CRGB::Blue,
	    CRGB::Black,
	    CRGB::Black
	};
};



#endif /* ANIMATION_COLORPALETTEANIMATION_H_ */
