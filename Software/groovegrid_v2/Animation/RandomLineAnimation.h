

#ifndef ANIMATION_RANDOMLINEANIMATION_H_
#define ANIMATION_RANDOMLINEANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/Animation.h"

class RandomLineAnimation: public Animation
{
public:
	RandomLineAnimation(GridTile* gridTile);
	~RandomLineAnimation();
	void run();
};



#endif /* ANIMATION_RANDOMLINEANIMATION_H_ */
