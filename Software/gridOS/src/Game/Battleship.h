/*
 * GAME_BATTLESHIP.h
 *
 *  Created on: 11.01.2019
 *      Author: pmale
 */

#ifndef GAME_BATTLESHIP
#define GAME_BATTLESHIP

#include "../PLATFORM.h"
#include "../utils/GrooveGame.h"
#include "../driver/BluetoothService.h"
#include <list>

class Battleship : public GrooveGame
{
public:
	Battleship(GridTile *tile);
	~Battleship();
	void start();
	void stop();
	void run();
	void draw();
	void drawCrosshair(uint8_t x, uint8_t y);
	void drawWater();
	void drawAllShips();
	void onCommand(CommandMsg *msg);
    GrooveApp* new_instance(GridTile *tile);
	void writePixel(uint8_t x, uint8_t y, CRGB color);

private:
	void reset();

	#define NR_SHIPS 5
	#define SIZE_X 10
	#define SIZE_Y 7
	#define SIZE_MIDBORDER 1

	CRGBPalette16 waterPalette =
	{
		CRGB::MidnightBlue,
		CRGB::DarkBlue,
		CRGB::DarkCyan,
		CRGB::MidnightBlue
	};
	uint8_t colorIndex = 0, motionStep = 0;

	typedef struct {
		uint8_t id;
		uint8_t x;
		uint8_t y;
		uint8_t len;
		uint8_t rot;
	}Ship;

	struct {
		uint8_t x;
		uint8_t y;
	}crosshair;

	enum {
		STATE_WAIT_FOR_SHIPS,
		STATE_CROSSHAIR,
		STATE_SHOOTING
	}gameState;

	std::list<Ship> shipList1, shipList2;
	CRGB shipColor = 0x037d00;
	DeltaTimer crosshairTimer;

};



#endif /* GAME_BATTLESHIP */
