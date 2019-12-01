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
#include "../driver/Microphone.h"

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
	#define BAR_NR 14
	struct Bar{
		CRGB color;
		uint8_t len;
		uint8_t xPos;
	};

	Bar barArray[BAR_NR];
	double fftBuffer[NR_FFT_SAMPLES];
	DeltaTimer barTimer;
	CRGBPalette16 lavaPalette =
	{
    	CRGB::White,
    	CRGB::Orange,
    	CRGB::Red,
    	CRGB::DarkRed
	};
};



#endif /* ANIMATION_SPECTRUMANIMATION_H_ */
