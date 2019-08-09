/*
 * NFSAnimation.h
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#ifndef ANIMATION_NFSANIMATION_H_
#define ANIMATION_NFSANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class NFSAnimation: public GrooveAnimation
{
public:
	NFSAnimation(GridTile* gridTile);
	~NFSAnimation();
	void run();
	GrooveApp* new_instance(GridTile *tile);
private:
	uint32_t animationSpeed;
	uint8_t **screenBuffer;
};



#endif /* ANIMATION_NFSANIMATION_H_ */
