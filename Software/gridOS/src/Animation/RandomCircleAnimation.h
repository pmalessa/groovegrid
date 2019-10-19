/*
 * RandomCircleAnimation.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_RANDOMCIRCLEANIMATION_H_
#define ANIMATION_RANDOMCIRCLEANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class RandomCircleAnimation: public GrooveAnimation
{
public:
	RandomCircleAnimation(GridTile* gridTile);
	~RandomCircleAnimation();
	void run();
private:
	uint32_t animationSpeed;
};



#endif /* ANIMATION_RANDOMCIRCLEANIMATION_H_ */
