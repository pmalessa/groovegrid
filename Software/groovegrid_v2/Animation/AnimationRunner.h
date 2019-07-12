/*
 * AnimationRunner.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_ANIMATIONRUNNER_H_
#define ANIMATION_ANIMATIONRUNNER_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"
#include "../utils/GrooveApp.h"
#include <queue>


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
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
