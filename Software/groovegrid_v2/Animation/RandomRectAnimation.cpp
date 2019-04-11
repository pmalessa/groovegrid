/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomRectGrooveAnimation.h"

RandomRectAnimation::RandomRectAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
}

RandomRectAnimation::~RandomRectAnimation()
{
}

void RandomRectAnimation::run()
{
	static uint8_t init = 0, up = 1;
	static uint8_t r,g,b,x,y,w,h, r_cur, g_cur, b_cur, speed;

	if(init)
	{
		init = 0;
		up = 1;
		r = rand()%256; g = rand()%256; b = rand()%256;
		x = rand()%XMAX; y = rand()%YMAX; w = rand()%XMAX; h = rand()%YMAX;
		speed = (rand()%3) +1;
		r_cur = 0; g_cur = 0; b_cur = 0;
	}

	if(up)	//clear or draw
	{
		tile->fillRect(x, y, w, h, tile->RGB(r_cur, g_cur, b_cur));
		if((uint16_t)r_cur+speed > r) r_cur = r; else r_cur += speed;
		if((uint16_t)g_cur+speed > g) g_cur = g; else g_cur += speed;
		if((uint16_t)b_cur+speed > b) b_cur = b; else b_cur += speed;
		if(r_cur == r && g_cur == g && b_cur == b)
		{
			up = 0;
		}
	}
	else
	{
		tile->fillRect(x, y, w, h, tile->RGB(r, g, b));
		if((int8_t)r-speed < 0) r=0; else r -= speed;
		if((int8_t)g-speed < 0) g=0; else g -= speed;
		if((int8_t)b-speed < 0) b=0; else b -= speed;
		if(r == 0 && g == 0 && b == 0)
		{
			init = 1;
		}
	}
}
