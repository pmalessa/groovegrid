/*
 * FlappyGroove.cpp
 *
 *  Created on: 28.04.2019
 *      Author: pmale
 */

#include "FlappyGroove.h"

FlappyGroove::FlappyGroove(GridTile *tile):GrooveGame(tile)
{
	gameSpeed = 5;		 //new gameFrame after X frames
	worldMoveSpeed = 10; //move world after X gameFrames
	wallProbability = 25; //percent
	frameTimer.setTimeStep(FRAMERATE_MS*gameSpeed);
	gameState = new FlappyGrooveState();

	gameState->xmax = tile->getWidth()-1;	//getting board size
	gameState->ymax = tile->getHeight()-2;  //1 pixel for floor
	maxWallHeight = (gameState->ymax+1)/3;  //one third for wall and gap

	Serial.println(gameState->xmax);
	Serial.println(gameState->ymax);
	Serial.println(maxWallHeight);

	gameState->board=new boardField*[gameState->xmax];	//initialize board
	for(int i=0;i<gameState->xmax;i++)
	{
		gameState->board[i]=new boardField[gameState->ymax];
	}
	initBoard();
	Serial.println("Const done");
}

FlappyGroove::~FlappyGroove()
{
	delete gameState;
}

void FlappyGroove::run()
{
	static uint8_t worldMoveCounter = worldMoveSpeed;
	if(frameTimer.isTimeUp())
	{
		drawBoard();
		Serial.println("f");
		if(worldMoveCounter == 0)
		{
			Serial.println("w");
			worldMoveCounter = worldMoveSpeed;
			moveWorld();
		}
		else
		{
			worldMoveCounter--;
		}
	}
}

//spawn wall at the right corner
void FlappyGroove::spawnWall()
{
	uint8_t topWall = rand()%maxWallHeight;
	uint8_t botWall = rand()%maxWallHeight;

	for(uint8_t i = 0;i<gameState->ymax;i++)
	{
		if(i <= topWall)	//draw top wall
		{
			gameState->board[gameState->xmax][i] = FIELD_WALL;
		}
		else if(i >= gameState->ymax-botWall)	//draw bot wall
		{
			gameState->board[gameState->xmax][i] = FIELD_WALL;
		}
		else //draw space
		{
			gameState->board[gameState->xmax][i] = FIELD_SPACE;
		}
	}
}
void FlappyGroove::moveWorld()
{
	for(uint8_t i = 0; i <= gameState->ymax;i++)	//for each row
	{
		for(uint8_t j = 1; j <=gameState->xmax; j++)	//move each block to the left, start with block 1 instead of 0
		{
			gameState->board[j-1][i] = gameState->board[j][i];
		}
	}
	if(rand()%100 < wallProbability)
	{
		spawnWall();
	}
	else	//draw empty field
	{
		for(uint8_t i = 0; i <= gameState->ymax;i++)	//for each row
		{
			gameState->board[gameState->xmax][i] = FIELD_EMPTY;
		}
	}
}

void FlappyGroove::initBoard()
{
	for(uint8_t i=0;i<gameState->xmax;i++)
	{
		for(uint8_t j=0; j<gameState->ymax; j++)
		{
			gameState->board[i][j] = FIELD_EMPTY;
		}
	}
}

void FlappyGroove::drawBoard()
{
	tile->startWrite();
	for(uint8_t i=0;i<gameState->xmax;i++)
	{
		for(uint8_t j=0; j<gameState->ymax; j++)
		{
			switch (gameState->board[i][j]) {
				case FIELD_SPACE:
				case FIELD_EMPTY:
					tile->writePixel(i, j, tile->RGB(0, 0, 0));
					break;
				case FIELD_WALL:
					tile->writePixel(i, j, tile->RGB(255, 0, 0));
					break;
				case FIELD_PLAYER:
					tile->writePixel(i, j, tile->RGB(0, 0, 255));
					break;
				default:
					break;
			}
		}
	}
	tile->endWrite();
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



