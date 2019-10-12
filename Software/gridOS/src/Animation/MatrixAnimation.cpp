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
	static MatrixSnake newsnake;
	if(frameTimer.isTimeUp())
	{
		tile->fillScreenBuffer(CRGB(0,0,0));
		//generate new MatrixSnake
		if(esp_random()%100 <= MATRIX_NEW_SNAKE_PROB)
		{
			newsnake.x = tile->getWidth()-1;
			newsnake.y = esp_random()%tile->getHeight();
			newsnake.length = esp_random()%tile->getWidth();
			snakeList.push_back(newsnake);
		}

		//move snakes further
		if(!snakeList.empty())	//if list is empty, leave
		{
			std::list<MatrixSnake>::iterator it = snakeList.begin();
			do
			{
				//draw snakes
				uint16_t x2 = std::min(it->x + it->length,tile->getWidth()-1);	//limit width
				tile->writeLine(it->x, it->y, x2, it->y, CRGB(0,255,0));
				if(it->x > 0)	//if head not yet at the end, move further
				{
					it->x--;
					it++;
				}
				else if(it->length > 0)	//if head passed the end, but the body is still in frame, shorten length
				{
					it->length--;
					it++;
				}
				else
				{
					it = snakeList.erase(it);	//else remove MatrixSnake, save new iterator pointing to next element
				}
			}while(it != snakeList.end());
			tile->endWrite();
		}
	}
}

