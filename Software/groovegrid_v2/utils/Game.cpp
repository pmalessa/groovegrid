/*
 * Game.cpp
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#include "Game.h"
#include "TaskScheduler.h"

Game::Game()
{

}

void Game::run()
{
	//Game Loop
}

void Game::start()
{
}

void Game::stop()
{
	static TaskScheduler& taskScheduler = TaskScheduler::getInstance();
	taskScheduler.Detach(this);
}

void Game::reset()
{

}
