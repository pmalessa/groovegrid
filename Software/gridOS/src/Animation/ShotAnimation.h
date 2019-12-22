/*
 * ShotAnimation.h
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#ifndef ANIMATION_SHOTANIMATION_H_
#define ANIMATION_SHOTANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class ShotAnimation: public GrooveAnimation
{
public:
	ShotAnimation(GridTile* gridTile);
	~ShotAnimation();
	void run();
private:
	uint32_t animationSpeed;
	uint8_t textHeight, textSize, textOffset, textSpacing;
	CRGB textColor, bgColor;
	DeltaTimer animationTimer;
};



#endif /* ANIMATION_SHOTANIMATION_H_ */
