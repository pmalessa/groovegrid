
#ifndef ANIMATION_RANDOMRECTSANIMATION_H_
#define ANIMATION_RANDOMRECTSANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/Animation.h"

class RandomRectsAnimation: public Animation
{
public:
	RandomRectsAnimation(GridTile* gridTile);
	void run();
};



#endif /* ANIMATION_RANDOMRECTSANIMATION_H_ */
