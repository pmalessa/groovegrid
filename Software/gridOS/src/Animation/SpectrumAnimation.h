/*
 * SpectrumAnimation.h
 *
 *  Created on: 30.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_SPECTRUMANIMATION_H_
#define ANIMATION_SPECTRUMANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class SpectrumAnimation : public GrooveAnimation
{
public:
	SpectrumAnimation(GridTile *tile);
	void run();
private:
	void initBars();
	void drawBars();
	void updateBars();
	#define BAR_WIDTH 3
	#define BAR_NR 12
	struct Bar{
		CRGB color;
		uint8_t len;
		uint8_t xPos;
	};

	Bar barArray[BAR_NR];
	DeltaTimer barTimer;
};



#endif /* ANIMATION_SPECTRUMANIMATION_H_ */
