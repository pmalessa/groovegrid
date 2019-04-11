/*
 * AnimationRunner.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_GROOVEANIMATIONRUNNER_H_
#define ANIMATION_GROOVEANIMATIONRUNNER_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"
#include "../utils/GrooveApp.h"
#include "RandomRectGrooveAnimation.h"


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
	uint32_t animationStartTime;
	uint8_t selectedAnimation;
	GrooveAnimation* currentAnimation;
};



#endif /* ANIMATION_GROOVEANIMATIONRUNNER_H_ */
