/*
 * AnimationRunner.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "AnimationRunner.h"

std::map<String, std::function<GrooveAnimation*(GridTile*)>> AnimationMap::animationMap = {
	{"Color Palette"		,[](GridTile *tile){return new ColorPaletteAnimation(tile);}},
	{"Simply Red"			,[](GridTile *tile){return new SimplyRedAnimation(tile);}},
	{"Dancefloor"			,[](GridTile *tile){return new RandomRectAnimation(tile);}},
	{"Matrix"				,[](GridTile *tile){return new MatrixAnimation(tile);}},
	{"Need for Speed"		,[](GridTile *tile){return new NFSAnimation(tile);}},
	{"Spectrum"				,[](GridTile *tile){return new SpectrumAnimation(tile);}}
};

AnimationRunner::AnimationRunner(GridTile* gridTile):GrooveApp(gridTile)
{
	currentAnimation = 0;
	repeating = true;
	tag = "AnimationRunner";
}

AnimationRunner::~AnimationRunner()
{
	Serial.println("Ani kill");
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
		setAnimation(DEFAULT_ANIMATION);
		animationTimer.setTimeStep(animationQueue.front()->animationLength);
	}
	Serial.println("Ani Start");
	Task::start();
}

void AnimationRunner::stop()
{
	Serial.println("Ani Stop");
	Task::stop();
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
	if(AnimationMap::animationMap.find(animationName) != AnimationMap::animationMap.end())
	{
		clearQueue();
		AnimationEntry *entry = new AnimationEntry();
		entry->animationPtr = AnimationMap::animationMap.at(animationName).operator()(tile);
		entry->animationLength = -1;	//ANIMATION_RUNTIME_MS
		animationQueue.push(entry);
	}
	else
	{
		return;
	}
}

void AnimationRunner::run()
{
	while(1)
	{
		if(animationTimer.isTimeUp())
		{
			if(repeating == true)
			{
				animationQueue.push(animationQueue.front()); //put first element to the back
			}
			else
			{	//delete element
				delete animationQueue.front()->animationPtr;
				delete animationQueue.front();
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
		if(!animationQueue.empty())
		{
			if(animationQueue.front()->animationPtr != nullptr)
			{
				animationQueue.front()->animationPtr->run();
			}
		}
		vTaskDelay(FRAMERATE_TICKS);
	}
}

void AnimationRunner::onCommand(DynamicJsonDocument doc, uint8_t channelID)
{
	String move = doc["move"].as<String>();
}
