/*
 * Transition_Boot.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef ANIMATION_BOOTGROOVETRANSITION_H_
#define ANIMATION_BOOTTRANSITION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveTransition.h"


class BootTransition: public GrooveTransition
{
public:
	BootTransition(GridTile* gridTile);
	void run();
private:
	uint16_t delayFrames, x0, x1, state;
};



#endif /* ANIMATION_BOOTGROOVETRANSITION_H_ */
