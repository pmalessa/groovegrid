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
};



#endif /* ANIMATION_BOOTGROOVETRANSITION_H_ */