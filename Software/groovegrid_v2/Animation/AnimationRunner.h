/*
 * AnimationRunner.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_ANIMATIONRUNNER_H_
#define ANIMATION_ANIMATIONRUNNER_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"
#include "../utils/GrooveApp.h"
#include "RandomRectAnimation.h"


class AnimationRunner: public GrooveApp
{
public:
	AnimationRunner(GridTile* gridTile);
	~AnimationRunner();
	void run();
	void start();
	void stop();
	void onInput(char *data){UNUSED(data);};
private:
	uint8_t selectedAnimation;
	GrooveAnimation* currentAnimation;
	DeltaTimer animationTimer;
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
