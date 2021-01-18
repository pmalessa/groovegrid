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
#include "../MessageService.h"
#include "AnimationMap.h"


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
private:

    struct AnimationEntry{
    	GrooveAnimation* animationPtr;
    	int32_t animationLength;
    };

    std::queue<AnimationEntry *> animationQueue;
    bool repeating;
    uint8_t currentAnimation;
	DeltaTimer animationTimer;

	#define DEFAULT_ANIMATION "Unicolor"
	#define ANIMATION_RUNTIME_MS 10000
};

#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
