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
	GrooveApp* new_instance(GridTile *tile);
private:
	void FillLEDsFromPaletteColors( uint8_t colorIndex);
	void ChangePalettePeriodically();

	CRGBPalette16 currentPalette;
	TBlendType    currentBlending;
};



#endif /* ANIMATION_COLORPALETTEANIMATION_H_ */
