/*
 * WalkingDotAnimation.h
 *
 *  Created on: 01.06.2019
 *      Author: pmale
 */

#ifndef ANIMATION_WALKINGDOTANIMATION_H_
#define ANIMATION_WALKINGDOTANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class WalkingDotAnimation: public GrooveAnimation
{
public:
	WalkingDotAnimation(GridTile* gridTile);
	~WalkingDotAnimation();
	void run();
private:
	void FillLEDsFromPaletteColors( uint8_t colorIndex);
	void ChangePalettePeriodically();

	void SetupTotallyRandomPalette();
	void SetupBlackAndWhiteStripedPalette();
	void SetupPurpleAndGreenPalette();
	CRGBPalette16 currentPalette;
	TBlendType    currentBlending;
	const TProgmemPalette16 myRedWhiteBluePalette_p =
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



#endif /* ANIMATION_WALKINGDOTANIMATION_H_ */
