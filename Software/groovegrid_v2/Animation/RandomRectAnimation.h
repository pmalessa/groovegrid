
#ifndef ANIMATION_RANDOMRECTANIMATION_H_
#define ANIMATION_RANDOMRECTANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"

class RandomRectAnimation: public GrooveAnimation
{
public:
	RandomRectAnimation(GridTile* gridTile);
	~RandomRectAnimation();
	void run();
	GrooveApp* new_instance(GridTile *tile);
private:
	void generateNewRect();

	bool fadeIn;
	uint8_t currentRed, currentGreen, currentBlue;
	uint8_t desiredRed, desiredGreen, desiredBlue;
	uint16_t x,y,w,h;
	uint16_t stepSize;
};



#endif /* ANIMATION_RANDOMRECTANIMATION_H_ */
