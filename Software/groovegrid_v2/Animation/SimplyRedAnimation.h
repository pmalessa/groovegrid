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
    GrooveApp* new_instance(GridTile *tile);
private:
	uint32_t genFlashInterval();

	uint8_t gameState = 0;
	uint16_t fadePosition = 0;
	uint16_t fadeDuration = 1000; //ms
	uint16_t flashDuration = 50; //ms
	uint32_t minFlashInterval = 100; //seconds
	uint32_t maxFlashInterval = 3000; //seconds
};



#endif /* ANIMATION_SIMPLYREDANIMATION_H_ */
