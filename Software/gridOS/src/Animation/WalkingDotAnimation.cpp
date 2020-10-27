/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "WalkingDotAnimation.h"

WalkingDotAnimation::WalkingDotAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
}

WalkingDotAnimation::~WalkingDotAnimation()
{

}

void WalkingDotAnimation::run()
{
    static uint8_t direction = 0;
    static uint16_t xIndex = 0;
    static uint8_t yIndex = 0;
    static uint8_t delay = 0;
    CRGBW color = CRGBW(255, 0, 0, 0);

	if(frameTimer.isTimeUp())
	{
        if(delay++ > 3)
        {
            delay = 0;
            
            switch (direction)
            {
            case 0:
                if(xIndex < tile->getWidth()-1)
                {
                    xIndex++;
                }
                else
                {
                    if(yIndex < tile->getHeight()-1)
                    {
                        yIndex++;
                        direction = 1;
                    }
                    else
                    {
                        xIndex = 0;
                        yIndex = 0;
                    }
                }
                break;
            case 1:
                if(xIndex > 0)
                {
                    xIndex--;
                }
                else
                {
                    if(yIndex < tile->getHeight()-1)
                    {
                        yIndex++;
                        direction = 0;
                    }
                    else
                    {
                        xIndex = 0;
                        yIndex = 0;
                    }
                    
                }
                break;
            }
            
            tile->fillScreenBuffer(CRGBW(0));
            tile->writePixel(xIndex,yIndex,color);
            tile->endWrite();
        }
    }
}