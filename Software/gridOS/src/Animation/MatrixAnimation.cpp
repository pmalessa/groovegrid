/*
 * MatrixAnimation.cpp
 *
 *  Created on: 09.08.2019
 *      Author: pmale
 */

#include "MatrixAnimation.h"

MatrixAnimation::MatrixAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	animationSpeed = 20;
}

MatrixAnimation::~MatrixAnimation()
{
}

void MatrixAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		tile->fillScreenBuffer(CRGB(0,0,0));
		//generate new snake
		if(esp_random()%100 <= MATRIX_NEW_SNAKE_PROB)
		{
			snake *newsnake = new snake;
			newsnake->x = tile->getWidth()-1;
			newsnake->y = esp_random()%tile->getHeight();
			newsnake->length = esp_random()%tile->getWidth();
			snakeList.push_back(newsnake);
		}

		//move snakes further
		for(std::list<snake*>::iterator it = snakeList.begin();it != snakeList.end(); ++it)
		{
			//draw snakes
			snake* curSnake = *it;
			uint16_t x2 = std::min(curSnake->x+curSnake->length,tile->getWidth()-1);	//limit width
			tile->writeLine(curSnake->x, curSnake->y, x2, curSnake->y, CRGB(0,255,0));
			if(curSnake->x > 0)
			{
				curSnake->x--;
			}
			else if(curSnake->length > 0)
			{
				curSnake->length--;
			}
			else
			{
				delete curSnake;
				snakeList.erase(it);	//remove snake
			}
		}
	}
}

