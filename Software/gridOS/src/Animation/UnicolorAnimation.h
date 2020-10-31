/*
 * UnicolorAnimation.h
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#ifndef ANIMATION_UNICOLORANIMATION_H_
#define ANIMATION_UNICOLORANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class UnicolorAnimation: public GrooveAnimation{
public:
	UnicolorAnimation(GridTile *tile);
	~UnicolorAnimation();
	void run();
	void start();
	void stop();
private:

};


#endif /* ANIMATION_UNICOLORANIMATION_H_ */
