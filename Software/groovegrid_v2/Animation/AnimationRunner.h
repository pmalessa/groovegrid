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
    std::string onUserRead(uint8_t channelID);
    void onUserWrite(std::string data, uint8_t channelID);
    GrooveApp* new_instance(GridTile *tile);
private:

    struct AnimationEntry{
    	GrooveAnimation* animationPtr;
    	uint32_t animationLength;
    };

    std::vector<AnimationEntry *> animationQueue;
    bool repeating;
    uint8_t currentAnimation;
	DeltaTimer animationTimer;
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
