/*
 * AnimationRunner.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_ANIMATIONRUNNER_H_
#define ANIMATION_ANIMATIONRUNNER_H_

#include "../PLATFORM.h"
#include "../utils/GrooveApp.h"
#include <queue>

#include "RandomLineAnimation.h"
#include "RandomPixelAnimation.h"
#include "RandomRectAnimation.h"
#include "RandomCircleAnimation.h"
#include "ColorPaletteAnimation.h"
#include "NFSAnimation.h"
#include "SimplyRedAnimation.h"
#include "MatrixAnimation.h"
#include "SpectrumAnimation.h"


class AnimationRunner: public GrooveApp
{
public:
	AnimationRunner(GridTile* gridTile);
	~AnimationRunner();
	void run();
	void start();
	void stop();
	void setAnimation(std::string animationName);
	void clearQueue();
	void onCommand(DynamicJsonDocument doc, uint8_t channelID);
    GrooveApp* new_instance(GridTile *tile);
private:

    struct AnimationEntry{
    	GrooveAnimation* animationPtr;
    	int32_t animationLength;
    };

    std::queue<AnimationEntry *> animationQueue;
    bool repeating;
    uint8_t currentAnimation;
	DeltaTimer animationTimer;

	#define DEFAULT_ANIMATION "Color Palette"
	#define ANIMATION_RUNTIME_MS 10000
};

class AnimationMap{
public:
	static std::map<std::string, std::function<GrooveAnimation*(GridTile*)>> animationMap;
	static void add(std::string animationName, std::function<GrooveAnimation*(GridTile*)> returnObjectFunction)
	{
		animationMap.emplace(animationName,returnObjectFunction);
	};
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
