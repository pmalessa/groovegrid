/*
 * RandomPixelAnimation.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_RANDOMPIXELANIMATION_H_
#define ANIMATION_RANDOMPIXELANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/Animation.h"

class RandomPixelAnimation: public Animation
{
public:
	RandomPixelAnimation(GridTile* gridTile);
	~RandomPixelAnimation();
	void run();
};



#endif /* ANIMATION_RANDOMPIXELANIMATION_H_ */
