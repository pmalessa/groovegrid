/*
 * MatrixAnimation.h
 *
 *  Created on: 09.08.2019
 *      Author: pmale
 */

#ifndef ANIMATION_MATRIXANIMATION_H_
#define ANIMATION_MATRIXANIMATION_H_

#include "../PLATFORM.h"
#include "../utils/GrooveAnimation.h"
#include <list>

#define MATRIX_NEW_SNAKE_PROB 10  //percent

class MatrixAnimation: public GrooveAnimation
{
public:
	MatrixAnimation(GridTile* gridTile);
	~MatrixAnimation();
	void run();
private:
	uint32_t animationSpeed;
	typedef struct{
		uint16_t x;
		uint16_t y;
		uint16_t length;
	}MatrixSnake;

	std::list<MatrixSnake> snakeList;
};



#endif /* ANIMATION_MATRIXANIMATION_H_ */
