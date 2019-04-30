/*
 * AnimationRunner.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "AnimationRunner.h"

#include "BootTransition.h"
#include "RandomLineAnimation.h"
#include "RandomPixelAnimation.h"
#include "RandomRectAnimation.h"

#define ANIMATION_RUNTIME_MS 10000

AnimationRunner::AnimationRunner(GridTile* gridTile):GrooveApp(gridTile)
{
	currentAnimation = 0;
	repeating = true;
}

AnimationRunner::~AnimationRunner()
{

}

void AnimationRunner::start()
{
	if(animationQueue.empty())
	{
		//init default Queue
		AnimationEntry *entry;
		entry = new AnimationEntry();
		entry->animationPtr = new RandomRectAnimation(tile);
		entry->animationLength = ANIMATION_RUNTIME_MS;
		animationQueue.push_back(entry);

		entry = new AnimationEntry();
		entry->animationPtr = new RandomPixelAnimation(tile);
		entry->animationLength = ANIMATION_RUNTIME_MS;
		animationQueue.push_back(entry);

		entry = new AnimationEntry();
		entry->animationPtr = new RandomLineAnimation(tile);
		entry->animationLength = ANIMATION_RUNTIME_MS;
		animationQueue.push_back(entry);

		currentAnimation = 0;
		animationTimer.setTimeStep(animationQueue.at(currentAnimation)->animationLength);
	}
}

void AnimationRunner::stop()
{
}

void AnimationRunner::run()
{
	if(animationTimer.isTimeUp())
	{
		currentAnimation++;//next animation
		if(animationQueue.size() > currentAnimation)	//element available
		{
			animationTimer.setTimeStep(animationQueue.at(currentAnimation)->animationLength);
		}
		else
		{
			if(repeating == true)
			{
				currentAnimation = 0;
				animationTimer.setTimeStep(animationQueue.at(currentAnimation)->animationLength);
			}
			else
			{
				animationQueue.clear();
				tile->fillScreen(tile->RGB(0));
			}
		}
	}
	if(frameTimer.isTimeUp())
	{
		if(!animationQueue.empty())
		{
			if(animationQueue.at(currentAnimation)->animationPtr != nullptr)
			{
				animationQueue.at(currentAnimation)->animationPtr->run();
			}
		}
	}
}

std::string AnimationRunner::onUserRead(uint8_t channelID)
{
	return 0;
}
void AnimationRunner::onUserWrite(std::string data, uint8_t channelID)
{
	UNUSED(data);
	UNUSED(channelID);
}
