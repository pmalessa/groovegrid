/*
 * RandomPixelAnimation.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_RANDOMPIXELANIMATION_H_
#define ANIMATION_RANDOMPIXELANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class RandomPixelAnimation: public GrooveAnimation
{
public:
	RandomPixelAnimation(GridTile* gridTile);
	~RandomPixelAnimation();
	void run();
private:
	uint32_t animationStartTime;
	uint32_t animationSpeed;
};



#endif /* ANIMATION_RANDOMPIXELANIMATION_H_ */
