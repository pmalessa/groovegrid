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
	void onCommand(CommandMsg *msg);

private:
	void draw();
	void moveCrosshair(std::string cmd, uint8_t playerID);
	void drawCrosshair(uint8_t x, uint8_t y, uint8_t playerID);
	void drawMidBorder();
	void drawWater();
	void drawAllShips();
	bool shoot();	//true if hit
	bool isShipThere(uint8_t block_x, uint8_t block_y, uint8_t playerID);
	void writeBlock(uint8_t x, uint8_t y, uint8_t playerID, CRGB color);
	void writePixel(uint8_t x, uint8_t y, uint8_t playerID, CRGB color);
	void drawHitmap();
	void setHitmap();
	void switchPlayer();
	bool shootAnimation();
	void generateShootLine();
	bool checkWin();

	#define NR_SHIPS 5
	#define GAMEFIELD_SIZE_BLOCKS_HEIGHT 10
	#define GAMEFIELD_SIZE_BLOCKS_WIDTH 7
	#define GAMEFIELD_SIZE_BLOCKS_MIDBORDER 1
	#define GAMEFIELD_BLOCK_TO_PIXEL 2
	#define HITANIMATION_DELAY 3500
	#define PLAYER1 1
	#define PLAYER2 2

	#define GAMEFIELD_SIZE_PIXEL_HEIGHT GAMEFIELD_SIZE_BLOCKS_HEIGHT*GAMEFIELD_BLOCK_TO_PIXEL
	#define GAMEFIELD_SIZE_PIXEL_WIDTH GAMEFIELD_SIZE_BLOCKS_WIDTH*GAMEFIELD_BLOCK_TO_PIXEL
	#define GAMEFIELD_SIZE_PIXEL_MIDBORDER GAMEFIELD_SIZE_BLOCKS_MIDBORDER*GAMEFIELD_BLOCK_TO_PIXEL

	#define CANNONBALL_MAX_HEIGHT 7

	#ifndef _swap_int16_t
	#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
	#endif

	CRGBPalette16 waterPalette =
	{
		CRGB::MidnightBlue,
		CRGB::DarkBlue,
		CRGB::DarkCyan,
		CRGB::MidnightBlue
	};
	uint8_t colorIndex = 0, motionStep = 0, currentPlayerID = 1;

	typedef struct {
		uint8_t id;
		uint8_t x;
		uint8_t y;
		uint8_t len;
		uint8_t rot;
		uint8_t hitmap;
		uint8_t sunken;
	}Ship;

	struct {
		uint8_t x;
		uint8_t y;
		uint8_t hit;
	}target;

	typedef struct{
		uint8_t x;
		uint8_t y;
	}CannonballPosition;

	struct {
		std::list<CannonballPosition> positionList;
		uint8_t r;
		float vel;
		float height;
	}cannonBall;

	enum {
		STATE_WAIT_FOR_SHIPS,
		STATE_CROSSHAIR,
		STATE_SHOOTING,
		STATE_WIN
	}gameState;

	std::list<Ship> shipList[2];
	CRGB shipColor = 0x037d00;
	CRGB shipSunkColor = 0xFF0000;
	CRGB hitColor = 0xFF00FF;
	CRGB waterShotColor = 0xFFFFFF;
	CRGB crosshairColor = 0xFF0000;
	DeltaTimer crosshairTimer, shootAnimationTimer;

	uint8_t waterShots[2][GAMEFIELD_SIZE_BLOCKS_WIDTH][GAMEFIELD_SIZE_BLOCKS_HEIGHT]; //2 Player, x, y
};



#endif /* GAME_BATTLESHIP */
