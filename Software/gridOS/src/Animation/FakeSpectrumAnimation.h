/*
 * FakeSpectrumAnimation.h
 *
 *  Created on: 30.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_FAKESPECTRUMANIMATION_H_
#define ANIMATION_FAKESPECTRUMANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"
#include "../driver/Microphone.h"

class FakeSpectrumAnimation : public GrooveAnimation
{
public:
	FakeSpectrumAnimation(GridTile *tile);
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



#endif /* ANIMATION_FAKESPECTRUMANIMATION_H_ */
