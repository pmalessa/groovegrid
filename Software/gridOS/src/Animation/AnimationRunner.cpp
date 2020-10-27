/*
 * AnimationRunner.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "AnimationRunner.h"

std::map<std::string, std::function<GrooveAnimation*(GridTile*)>> AnimationMap::animationMap = {
	{"Color Palette"		,[](GridTile *tile){return new ColorPaletteAnimation(tile);}},
	{"Simply Red"			,[](GridTile *tile){return new SimplyRedAnimation(tile);}},
	{"Dancefloor"			,[](GridTile *tile){return new RandomCircleAnimation(tile);}},
	{"Matrix"				,[](GridTile *tile){return new MatrixAnimation(tile);}},
	{"Need for Speed"		,[](GridTile *tile){return new NFSAnimation(tile);}},
	{"Spectrum"				,[](GridTile *tile){return new SpectrumAnimation(tile);}},
	{"Shot"					,[](GridTile *tile){return new ShotAnimation(tile);}},
	{"Walking Dot"			,[](GridTile *tile){return new WalkingDotAnimation(tile);}}
};

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
	currentAnimation = 0;
	if(animationQueue.empty())
	{
		setAnimation(DEFAULT_ANIMATION);
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

void AnimationRunner::setAnimation(std::string animationName)
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
	if(!animationQueue.empty())
	{
		if(animationQueue.front()->animationPtr != nullptr)
		{
			animationQueue.front()->animationPtr->run();
		}
	}
}

void AnimationRunner::onCommand(DynamicJsonDocument doc, uint8_t channelID)
{
	std::string move = doc["move"].as<std::string>();
}
