/*
 * AnimationRunner.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "AnimationRunner.h"
#include "../driver/Timer.h"
#include "../utils/TaskScheduler.h"
#include "RandomPixelAnimation.h"
#include "RandomLineAnimation.h"
#include "RandomRectAnimation.h"
#include "BootTransition.h"

#define ANIMATION_RUNTIME_MS 10000

AnimationRunner::AnimationRunner(GridTile* gridTile)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	UNUSED(tsched);
	tile = gridTile;
	animationStartTime = 0;
	selectedAnimation = 0;
	currentAnimation = new RandomRectAnimation(tile);


}

AnimationRunner::~AnimationRunner()
{

}

void AnimationRunner::start()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	tsched.Attach(currentAnimation);
}

void AnimationRunner::stop()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	tsched.Detach(currentAnimation);
}

void AnimationRunner::run()
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	if(Timer::getMillis()-animationStartTime > ANIMATION_RUNTIME_MS)
	{
		animationStartTime = Timer::getMillis();
		switch (selectedAnimation) {
			case 3:
				selectedAnimation = 0;
				//nobreak
			case 0:
				tsched.Detach(currentAnimation);
				delete currentAnimation;
				currentAnimation = new RandomPixelAnimation(tile);
				tsched.Attach(currentAnimation);
				break;
			case 1:
				tsched.Detach(currentAnimation);
				delete currentAnimation;
				currentAnimation = new RandomLineAnimation(tile);
				tsched.Attach(currentAnimation);
				break;
			case 2:
				tsched.Detach(currentAnimation);
				delete currentAnimation;
				currentAnimation = new RandomRectAnimation(tile);
				tsched.Attach(currentAnimation);
				break;
		}
		selectedAnimation++;
	}
}
