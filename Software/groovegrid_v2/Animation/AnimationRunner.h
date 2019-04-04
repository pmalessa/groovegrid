/*
 * AnimationRunner.h
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#ifndef ANIMATION_ANIMATIONRUNNER_H_
#define ANIMATION_ANIMATIONRUNNER_H_

#include "../PLATFORM.h"
#include "../utils/GroovegridApp.h"
#include "../utils/Animation.h"
#include "RandomRectsAnimation.h"


class AnimationRunner: public GroovegridApp
{
public:
	AnimationRunner(GridTile* gridTile);
	void run();

	void start(){};				//starts App
	void pause(){};				//pauses App
	void stop(){};				//stops App
	void reset(){}; 				//reset App to initial state
	char* exportAppState(){return 0;};
	void importAppState(char *input){UNUSED(input);};
	uint8_t getProgress(){return 0;};		//return Progress 0..100
	void onInput(char *data){UNUSED(data);};
private:
	uint32_t animationStartTime;
	uint8_t selectedAnimation;
	Animation* currentAnimation;
	RandomRectsAnimation& ani;
};



#endif /* ANIMATION_ANIMATIONRUNNER_H_ */
