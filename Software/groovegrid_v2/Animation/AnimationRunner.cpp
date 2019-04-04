/*
 * AnimationRunner.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "AnimationRunner.h"
#include "../driver/Timer.h"
#include "../utils/TaskScheduler.h"
#include "RandomPixelAnimation.h"
#include "RandomLineAnimation.h"
#include "RandomRectsAnimation.h"

AnimationRunner::AnimationRunner(GridTile* gridTile)
{
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	UNUSED(tsched);
	tile = gridTile;
	animationStartTime = 0;
	selectedAnimation = 0;


	//currentAnimation = &RandomPixelAnimation(tile);
	ani = RandomRectsAnimation(gridTile);
	ani.run();
}
void AnimationRunner::run()
{
	ani.run();

	tile->drawPixel(1, 2, tile->RGB(50, 122, 255));
	tile->drawPixel(2, 2, tile->RGB(50, 122, 255));
	tile->drawPixel(3, 2, tile->RGB(50, 122, 255));
	/*
	static TaskScheduler& tsched = TaskScheduler::getInstance();
	if(Timer::getMillis()-animationStartTime > 30000)
	{
		animationStartTime = Timer::getMillis();
		selectedAnimation++;
		switch (selectedAnimation) {
			case 0:
				*currentAnimation = RandomPixelAnimation(tile);
				tsched.Attach(currentAnimation);
				break;
			case 1:
				//*currentAnimation = RandomLineAnimation(tile);
				break;
			case 2:
				//*currentAnimation = RandomRectsAnimation(tile);
				break;
			default:
				selectedAnimation = 0;
				break;
		}
	}
	*/
}
