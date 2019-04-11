
#ifndef ANIMATION_RANDOMRECTGROOVEANIMATION_H_
#define ANIMATION_RANDOMRECTGROOVEANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class RandomRectAnimation: public GrooveAnimation
{
public:
	RandomRectAnimation(GridTile* gridTile);
	~RandomRectAnimation();
	void run();
};



#endif /* ANIMATION_RANDOMRECTGROOVEANIMATION_H_ */
