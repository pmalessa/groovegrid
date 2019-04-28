/*
 * FlappyGroove.cpp
 *
 *  Created on: 28.04.2019
 *      Author: pmale
 */

#include "FlappyGroove.h"

FlappyGroove::FlappyGroove(GridTile *tile):GrooveGame(tile)
{
	gameSpeed = 5;
	frameTimer.setTimeStep(FRAMERATE_MS*gameSpeed);
	gameState = new FlappyGrooveState();

	gameState->xmax = tile->getWidth()-1;	//getting board size
	gameState->ymax = tile->getHeight()-2;  //1 pixel for floor

	gameState->board=new uint8_t*[gameState->xmax];	//initialize board
	for(int i=0;i<gameState->xmax;i++)
	{
		gameState->board[i]=new uint8_t[gameState->ymax];
	}
}

FlappyGroove::~FlappyGroove()
{
	delete gameState;
}

void FlappyGroove::run()
{
	if(frameTimer.isTimeUp())
	{

	}
}
void FlappyGroove::start()
{
	tile->fillScreen(tile->RGB(0, 0, 0));
	tile->drawLine(0, gameState->ymax+1, gameState->xmax, gameState->ymax+1, tile->RGB(40, 125, 84));	//draw floor
}
void FlappyGroove::stop()
{

}
std::string FlappyGroove::onUserRead(uint8_t channelID)
{
	return 0;
}
void FlappyGroove::onUserWrite(std::string data, uint8_t channelID)
{
	UNUSED(channelID);
	switch (data.c_str()[0]) {
		case 'u':
			//move(UP);
			break;
		default:
			break;
	}
}



