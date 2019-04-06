/*
 * AnimationRunner.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_ANIMATIONRUNNER_H_
#define ANIMATION_ANIMATIONRUNNER_H_

#include "../PLATFORM.h"
#include "../utils/GroovegridApp.h"
#include "../utils/Animation.h"
#include "RandomRectAnimation.h"


class AnimationRunner: public GroovegridApp
{
public:
	AnimationRunner(GridTile* gridTile);
	~AnimationRunner();
	void run();
	void start();
	void stop();
	void onInput(char *data){UNUSED(data);};
private:
	uint32_t animationStartTime;
	uint8_t selectedAnimation;
	Animation* currentAnimation;
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
