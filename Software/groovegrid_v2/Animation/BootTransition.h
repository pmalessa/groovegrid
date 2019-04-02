/*
 * Transition_Boot.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef ANIMATION_BOOTTRANSITION_H_
#define ANIMATION_BOOTTRANSITION_H_

#include "../PLATFORM.h"
#include "../utils/Transition.h"


class BootTransition: public Transition
{
public:
	BootTransition(GridTile* gridTile);
	void run();
};



#endif /* ANIMATION_BOOTTRANSITION_H_ */
