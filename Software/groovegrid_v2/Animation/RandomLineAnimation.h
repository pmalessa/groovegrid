

#ifndef ANIMATION_RANDOMLINEANIMATION_H_
#define ANIMATION_RANDOMLINEANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class RandomLineAnimation: public GrooveAnimation
{
public:
	RandomLineAnimation(GridTile* gridTile);
	~RandomLineAnimation();
	void run();
	GrooveApp* new_instance(GridTile *tile);
private:
	uint32_t animationSpeed;
};



#endif /* ANIMATION_RANDOMLINEANIMATION_H_ */
