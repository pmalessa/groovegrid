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

void SnakeGame::initialize() {
	gameState = new SnakeGameState();
	gameState->head = new Coordinate(10, 3);
	for (uint8_t i=0;i < 5;++i) {
		gameState->body.push_front(new Coordinate(gameState->head->x - i - 1,gameState->head->y));
	}
	spawnFood();
}

void SnakeGame::start() {
	tile->fillScreen(tile->RGB(0));
}

void SnakeGame::stop() {

}

std::string SnakeGame::onUserRead(uint8_t channelID) {
	return "";
}

void SnakeGame::onUserWrite(std::string data, uint8_t channelID) {
	UNUSED(channelID);
	switch (data.c_str()[0]) {
			case 'u':
				direction = up;
				break;
			case 'd':
				direction = down;
				break;
			case 'r':
				direction = right;
				break;
			case 'l':
				direction = left;
				break;
			default:
				break;
		}
}

void SnakeGame::run() {
	if (frameTimer.isTimeUp()) {
		move();
		draw();
	}
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
	tile->startWrite();
	tile->fillScreenBuffer(tile->RGB(0)); // Clear Screen

	tile->writePixel(gameState->head->x, gameState->head->y, tile->RGB(color));
	drawElement(*food);

	for (uint8_t i=0;i < gameState->body.size() ; ++i) {
		drawElement(*gameState->body[i]);
		Serial.println("Body at index ");
		Serial.println(i);
		Serial.println("is at pixel: ");
		Serial.println(gameState->body[i]->x);
		Serial.println(", ");
		Serial.println(gameState->body[i]->y);
	}

	tile->endWrite();
}

void SnakeGame::drawElement(uint8_t x, uint8_t y) {
	tile->writePixel(x, y, tile->RGB(color));
}

void SnakeGame::drawElement(Coordinate coordinate) {
	drawElement(coordinate.x, coordinate.y);
}
