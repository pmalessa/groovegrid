

#ifndef ANIMATION_RANDOMLINEGROOVEANIMATION_H_
#define ANIMATION_RANDOMLINEGROOVEANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class RandomLineAnimation: public GrooveAnimation
{
public:
	RandomLineAnimation(GridTile* gridTile);
	~RandomLineAnimation();
	void run();
private:
	uint32_t animationStartTime;
	uint32_t animationSpeed;
};



#endif /* ANIMATION_RANDOMLINEGROOVEANIMATION_H_ */
