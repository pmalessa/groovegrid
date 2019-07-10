#include "Snake.h"

Coordinate::Coordinate(uint8_t x, uint8_t y) {
	this->x = x;
	this->y = y;
}

SnakeGame::SnakeGame(GridTile *tile): GrooveGame(tile) {
	frameTimer.setTimeStep(FRAMERATE_MS*frameDelay);
	initialize();
}

SnakeGame::~SnakeGame(){
	delete gameState;
}

GrooveApp* SnakeGame::new_instance(GridTile *tile)
{
	return new SnakeGame(tile);
}

void SnakeGame::initialize() {
	gameState = new SnakeGameState();
	gameState->head = new Coordinate(10, 3);
	for (uint8_t i=0;i < 5;++i) {
		gameState->body.push_front(new Coordinate(gameState->head->x - i - 1,gameState->head->y));
	}
	spawnFood();
}

void SnakeGame::start() {
	tile->fillScreen(CRGB(0));
}

void SnakeGame::stop() {

}

void SnakeGame::onCommand(DynamicJsonDocument doc, uint8_t channelID)
{
	UNUSED(channelID);
	String move = doc["move"].as<String>();

	if(move=="up")
	{
		direction = up;
	}
	else if(move=="down")
	{
		direction = down;
	}
	else if(move=="right")
	{
		direction = right;
	}
	else if(move=="left")
	{
		direction = left;
	}
}

void SnakeGame::run() {
	if (frameTimer.isTimeUp()) {
		move();
		if (detectCollision()) {
			direction = none;
		}
		draw();
		wrapAroundBorder();
	}
}

bool SnakeGame::detectCollision() {
	for (uint8_t i = 0; i < gameState->body.size(); ++i) {
		if (gameState->head->x == gameState->body[i]->x && gameState->head->y == gameState->body[i]->y) {
			return true;
		}
	}

	return false;
}

void SnakeGame::spawnFood() {
	uint8_t x = esp_random()%tile->getWidth();
	uint8_t y = esp_random()%tile->getHeight();

	if (x == gameState->head->x && y == gameState->head->y) {
		spawnFood();
	}

	for (uint8_t i = 0; i < gameState->body.size(); ++i) {
		if (x == gameState->body[i]->x && y == gameState->body[i]->y) {
			spawnFood();
		}
	}

	food = new Coordinate(x, y);
}

void SnakeGame::wrapAroundBorder() {
	if (gameState->head->x >= tile->getWidth() && direction == right) {
		gameState->head->x = 0;
	} else if (gameState->head->x == 0 && direction == left) {
		gameState->head->x = tile->getWidth()-1;

	}

	if (gameState->head->y >= tile->getHeight() && direction == down) {
		gameState->head->y = 0;
	} else if (gameState->head->y <= 0 && direction == up) {
		gameState->head->y = tile->getHeight()-1;
	}
}

void SnakeGame::move() {
	uint8_t xDelta = 0;
	uint8_t yDelta = 0;

	if (direction != none) {
		Coordinate* newPixel = new Coordinate(gameState->head->x, gameState->head->y);

		if (food->x != gameState->head->x || food->y != gameState->head->y) {
			gameState->body.pop_front();
		} else {
			// Eating food
			spawnFood();
		}

		gameState->body.push_back(newPixel);
	}

	switch (direction) {
				case up:
					yDelta = -1;
					break;
				case down:
					yDelta = 1;
					break;
				case right:
					xDelta = 1;
					break;
				case left:
					xDelta = -1;
					break;
				default:
					xDelta = 0;
					yDelta = 0;
					break;
			}
	gameState->head->x += xDelta;
	gameState->head->y += yDelta;
}

void SnakeGame::draw() {
	tile->fillScreenBuffer(CRGB(0)); // Clear Screen

	tile->writePixel(gameState->head->x, gameState->head->y, CRGB(color));
	drawElement(*food);

	for (uint8_t i=0;i < gameState->body.size() ; ++i) {
		drawElement(*gameState->body[i]);
	}

	tile->endWrite();
}

void SnakeGame::drawElement(uint8_t x, uint8_t y) {
	tile->writePixel(x, y, CRGB(color));
}

void SnakeGame::drawElement(Coordinate coordinate) {
	drawElement(coordinate.x, coordinate.y);
}
