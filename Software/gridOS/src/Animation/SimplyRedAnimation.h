/*
 * SimplyRedAnimation.h
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#ifndef ANIMATION_SIMPLYREDANIMATION_H_
#define ANIMATION_SIMPLYREDANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class SimplyRedAnimation: public GrooveAnimation{
public:
	SimplyRedAnimation(GridTile *tile);
	~SimplyRedAnimation();
	void run();
	void start();
	void stop();
private:
	uint32_t genFlashInterval();

	uint8_t gameState = 0;
	uint16_t fadePosition = 0;
	uint16_t fadeDuration = 1000; //ms
	DeltaTimer fadeTimer;
};



#endif /* ANIMATION_SIMPLYREDANIMATION_H_ */
