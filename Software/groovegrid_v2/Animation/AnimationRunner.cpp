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
#include "ColorPaletteAnimation.h"
#include "NFSAnimation.h"
#include "SimplyRedAnimation.h"

#define ANIMATION_RUNTIME_MS 10000

AnimationRunner::AnimationRunner(GridTile* gridTile):GrooveApp(gridTile)
{
	currentAnimation = 0;
	repeating = true;
}

AnimationRunner::~AnimationRunner()
{

}

GrooveApp* AnimationRunner::new_instance(GridTile *tile)
{
	return new AnimationRunner(tile);
}

void AnimationRunner::start()
{
	currentAnimation = 0;
	if(animationQueue.empty())
	{
		setAnimation("Color Palette");
		animationTimer.setTimeStep(animationQueue.front()->animationLength);
	}
}

void AnimationRunner::stop()
{
}

void AnimationRunner::clearQueue()
{
	while(!animationQueue.empty())
	{
		delete animationQueue.front()->animationPtr;
		delete animationQueue.front();
		animationQueue.pop();
	}
}

void AnimationRunner::setAnimation(String animationName)
{
	if(animationName == "Color Palette")
	{
		clearQueue();
		AnimationEntry *entry = new AnimationEntry();
		entry->animationPtr = new ColorPaletteAnimation(tile);
		entry->animationLength = -1;	//ANIMATION_RUNTIME_MS
		animationQueue.push(entry);
	}
	else if(animationName == "Dancefloor")
	{
		clearQueue();
		AnimationEntry *entry = new AnimationEntry();
		entry->animationPtr = new RandomRectAnimation(tile);
		entry->animationLength = 15000;//ANIMATION_RUNTIME_MS;
		animationQueue.push(entry);
		entry = new AnimationEntry();
		entry->animationPtr = new RandomLineAnimation(tile);
		entry->animationLength = 15000;//ANIMATION_RUNTIME_MS;
		animationQueue.push(entry);
		entry = new AnimationEntry();
		entry->animationPtr = new RandomPixelAnimation(tile);
		entry->animationLength = 15000;//ANIMATION_RUNTIME_MS;
		animationQueue.push(entry);
	}
	else if(animationName == "Need for Speed")
	{
		clearQueue();
		AnimationEntry *entry = new AnimationEntry();
		entry->animationPtr = new NFSAnimation(tile);
		entry->animationLength = -1;	//ANIMATION_RUNTIME_MS
		animationQueue.push(entry);
	}
	else if(animationName == "Simply Red")
	{
		clearQueue();
		AnimationEntry *entry = new AnimationEntry();
		entry->animationPtr = new SimplyRedAnimation(tile);
		entry->animationLength = -1;	//ANIMATION_RUNTIME_MS
		animationQueue.push(entry);
	}
	else
	{ //emergency animation
		clearQueue();
		AnimationEntry *entry = new AnimationEntry();
		entry->animationPtr = new ColorPaletteAnimation(tile);
		entry->animationLength = -1;	//ANIMATION_RUNTIME_MS
		animationQueue.push(entry);
	}
}

void AnimationRunner::run()
{
	if(animationTimer.isTimeUp())
	{
		if(repeating == true)
		{
			animationQueue.push(animationQueue.front()); //put first element to the back
		}
		animationQueue.pop();	//remove element
		if(!animationQueue.empty())	//element available
		{
			animationTimer.setTimeStep(animationQueue.front()->animationLength);
		}
		else
		{
			tile->fillScreen(CRGB(0));
		}
	}
	if(frameTimer.isTimeUp())
	{
		if(!animationQueue.empty())
		{
			if(animationQueue.front()->animationPtr != nullptr)
			{
				animationQueue.front()->animationPtr->run();
			}
		}
	}
}

void AnimationRunner::onCommand(DynamicJsonDocument doc, uint8_t channelID)
{
	String move = doc["move"].as<String>();
}
