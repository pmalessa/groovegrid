/*
 * ANIMATION.cpp
 *
 *  Created on: 22.09.2017
 *      Author: pmale
 */
#include"ANIMATION.h"

#include "../driver/GRID.h"



void ANIMATION_vBoot()
{
	static Grid& grid = Grid::getInstance();
	for (uint8_t i=0; i < YMAX; i++) {
		grid.drawLine(0, i, XMAX-1, i, grid.RGB(255, 0, 0));
		delay(100);
	}
	for (uint8_t i=0; i < YMAX; i++) {
		grid.drawLine(0, i, XMAX-1, i, grid.RGB(0, 0, 0));
		delay(100);
	}
}

void ANIMATION_randomPixels()
{
	static Grid& grid = Grid::getInstance();
	static uint32_t wait = 0;

	if(wait == 0)
	{
		if(rand()%2)	//clear or draw
		{
			grid.drawPixel(rand()%XMAX, rand()%YMAX, grid.RGB(rand()%256, rand()%256, rand()%256));
		}
		else
		{
			grid.drawPixel(rand()%XMAX, rand()%YMAX, grid.RGB(0, 0, 0));
		}

		wait = rand()%30;
	}
	else
	{
		wait--;
	}
}

void ANIMATION_randomPulseRects()
{
	static Grid& grid = Grid::getInstance();
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
		grid.fillRect(x, y, w, h, grid.RGB(r_cur, g_cur, b_cur));
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
		grid.fillRect(x, y, w, h, grid.RGB(r, g, b));
		if((int8_t)r-speed < 0) r=0; else r -= speed;
		if((int8_t)g-speed < 0) g=0; else g -= speed;
		if((int8_t)b-speed < 0) b=0; else b -= speed;
		if(r == 0 && g == 0 && b == 0)
		{
			init = 1;
		}
	}
}

void ANIMATION_randomLines()
{
	static Grid& grid = Grid::getInstance();
	static uint32_t wait = 0;

	if(wait == 0)
	{
		if(rand()%2)
		{
			grid.drawLine(rand()%XMAX, rand()%YMAX, rand()%XMAX, rand()%YMAX, grid.RGB(rand()%256, rand()%256, rand()%256));
		}
		else
		{
			grid.drawLine(rand()%XMAX, rand()%YMAX, rand()%XMAX, rand()%YMAX, grid.RGB(0, 0, 0));
		}

		wait = rand()%30;
	}
	else
	{
		wait--;
	}
}

void ANIMATION_vRunner()
{
	static Grid& grid = Grid::getInstance();
	static uint32_t wait = 3000;
	static uint8_t state = 0;
	if(wait == 0)
	{
		state++;	//switch Animation every 10 seconds
		grid.clearDisplay();
		wait = 3000;
	}
	wait--;
	switch (state) {
		case 0:
			ANIMATION_randomPixels();
			break;
		case 1:
			ANIMATION_randomLines();
			break;
		case 2:
			ANIMATION_randomPulseRects();
			break;
		default:
			state = 0;
			break;
	}
}
