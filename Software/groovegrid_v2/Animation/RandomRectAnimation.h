
#ifndef ANIMATION_RANDOMRECTANIMATION_H_
#define ANIMATION_RANDOMRECTANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/Animation.h"

class RandomRectAnimation: public Animation
{
public:
	RandomRectAnimation(GridTile* gridTile);
	~RandomRectAnimation();
	void run();
};



#endif /* ANIMATION_RANDOMRECTANIMATION_H_ */
