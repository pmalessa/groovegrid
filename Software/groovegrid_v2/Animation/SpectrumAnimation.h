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
	GrooveApp* new_instance(GridTile *tile);
	void generateBars();
	void drawBars();
private:
	void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);
	class Bar{
	public:
		uint16_t color;
		uint8_t xPos;
		uint8_t yPos;
	private:

	};
	Bar *barArray;
	uint8_t barNumber;
	uint8_t barWidth;
};



#endif /* ANIMATION_SPECTRUMANIMATION_H_ */
