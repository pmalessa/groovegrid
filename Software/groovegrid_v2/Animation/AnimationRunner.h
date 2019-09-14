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

#include "BootTransition.h"
#include "RandomLineAnimation.h"
#include "RandomPixelAnimation.h"
#include "RandomRectAnimation.h"
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
	void setAnimation(String animationName);
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
	std::map<String, std::function<GrooveAnimation*(GridTile*)>> animationMap = {
		{"Color Palette"		,[](GridTile *tile){return new ColorPaletteAnimation(tile);}},
		{"Simply Red"			,[](GridTile *tile){return new SimplyRedAnimation(tile);}},
		{"Dancefloor"			,[](GridTile *tile){return new RandomRectAnimation(tile);}},
		{"Matrix"				,[](GridTile *tile){return new MatrixAnimation(tile);}},
		{"Need for Speed"		,[](GridTile *tile){return new NFSAnimation(tile);}},
		{"Spectrum"				,[](GridTile *tile){return new SpectrumAnimation(tile);}}
	};

	#define DEFAULT_ANIMATION "Color Palette"
	#define ANIMATION_RUNTIME_MS 10000
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
