/*
 * FlappyGroove.cpp
 *
 *  Created on: 28.04.2019
 *      Author: pmale
 */

#include "FlappyGroove.h"

FlappyGroove::FlappyGroove(GridTile *tile):GrooveGame(tile)
{
	tag = "Flappy Groove";
	gameSpeed = 1;		 //new gameFrame after X frames
	worldMoveSpeed = 5; //move world after X gameFrames
	wallProbability = 15; //percent
	velocity = 0;
	gravity = 0.003;
	position = 0;
	playerPosition = 0;

	frameTimer.setTimeStep(FRAMERATE_MS*gameSpeed);
	gameState = new FlappyGrooveState();

	gameState->xmax = tile->getWidth()-1;	//getting board size
	gameState->ymax = tile->getHeight()-1;
	maxWallHeight = gameState->ymax/3;  //one third for wall and gap

	Serial.println(gameState->xmax);
	Serial.println(gameState->ymax);
	Serial.println(maxWallHeight);

	gameState->board=new boardField*[gameState->xmax+1];	//initialize board
	for(int i=0;i<=gameState->xmax;i++)
	{
		gameState->board[i]=new boardField[gameState->ymax+1];
	}
	initBoard();
}

FlappyGroove::~FlappyGroove()
{
	delete gameState;
}

GrooveApp* FlappyGroove::new_instance(GridTile *tile)
{
	return new FlappyGroove(tile);
}

void FlappyGroove::run()
{
	static uint8_t worldMoveCounter = worldMoveSpeed;

	while(1)
	{
		updatePlayer();
		drawBoard();
		if(worldMoveCounter == 0)
		{
			worldMoveCounter = worldMoveSpeed;
			moveWorld();
		}
		else
		{
			worldMoveCounter--;
		}
		if(isPlayerCollided())
		{
			restart();
		}
		vTaskDelay(FRAMERATE_TICKS);
	}
}

//spawn wall at the right corner
void FlappyGroove::spawnWall()
{
	uint8_t topWall = esp_random()%maxWallHeight;
	uint8_t botWall = esp_random()%maxWallHeight;

	for(uint8_t i = 0;i<=gameState->ymax;i++)
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
	if(esp_random()%100 < wallProbability)
	{
		spawnWall();
	}
	else	//draw empty field and floor
	{
		for(uint8_t i = 0; i < gameState->ymax;i++)	//for each row
		{
			gameState->board[gameState->xmax][i] = FIELD_EMPTY;
		}
		gameState->board[gameState->xmax][gameState->ymax] = FIELD_FLOOR;
	}
}

void FlappyGroove::initBoard()
{
	for(uint8_t i=0;i<=gameState->xmax;i++)
	{
		for(uint8_t j=0; j<gameState->ymax; j++)
		{
			gameState->board[i][j] = FIELD_EMPTY;
		}
		gameState->board[i][gameState->ymax] = FIELD_FLOOR;
	}
}

void FlappyGroove::drawBoard()
{
	for(uint8_t i=0;i<=gameState->xmax;i++)
	{
		for(uint8_t j=0; j<=gameState->ymax; j++)
		{
			switch (gameState->board[i][j]) {
				case FIELD_SPACE:
				case FIELD_EMPTY:
					tile->writePixel(i, j, CRGB(0, 0, 0));
					break;
				case FIELD_WALL:
					tile->writePixel(i, j, CRGB(255, 0, 0));
					break;
				case FIELD_FLOOR:
					tile->writePixel(i, j, CRGB(40, 125, 84));
					break;
				default:
					break;
			}
		}
	}
	playerPosition = (float) gameState->ymax * position;//draw Player
	tile->writePixel(0, gameState->ymax-1-playerPosition, CRGB(0, 0, 255));
	tile->endWrite();
}

bool FlappyGroove::isPlayerCollided()
{
	if(gameState->board[0][playerPosition] == FIELD_WALL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void FlappyGroove::updatePlayer()
{
	position += velocity;
	if(position <= 0)
	{
		position = 0;
		velocity = 0;
	}
	else
	{
		velocity -= gravity;        // Apply gravity to vertical velocity
	}
}

void FlappyGroove::start()
{
	tile->fillScreen(CRGB(0, 0, 0));
	Task::start();
}
void FlappyGroove::stop()
{
	Task::stop();
}

void FlappyGroove::restart()
{
	bool restartDone = 0;
	uint8_t i = 0;
	initBoard();
	position = 0;
	while(!restartDone)
	{
		if(i <= gameState->xmax)
		{
			tile->writeLine(i, 0, i, gameState->ymax, CRGB(255, 0, 0));
			tile->endWrite();

			i++;
		}
		else
		{
			restartDone = 1;
		}
		vTaskDelay(FRAMERATE_MS*gameSpeed);
	}
	restartDone = 0;
	i=0;
	while(!restartDone)
	{
		if(i <= gameState->xmax)
		{
			tile->writeLine(i, 0, i, gameState->ymax-1, CRGB(0, 0, 0));
			tile->writePixel(i, gameState->ymax, CRGB(40, 125, 84));
			tile->endWrite();
			i++;
		}
		else
		{
			restartDone = 1;
		}
		vTaskDelay(FRAMERATE_MS*gameSpeed);
	}
}

void FlappyGroove::onCommand(DynamicJsonDocument doc, uint8_t userID)
{
	UNUSED(userID);
	String move = doc["move"].as<String>();

	if(move=="up")
	{
		velocity = 0.03;
	}
}



