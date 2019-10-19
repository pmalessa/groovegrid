#include "Battleship.h"

Battleship::Battleship(GridTile *tile): GrooveGame(tile) {
	frameTimer.setTimeStep(FRAMERATE_MS);
	crosshairTimer.setTimeStep(150);
	gameState = STATE_WAIT_FOR_SHIPS;
	crosshair.x = 3;
	crosshair.y = 3;
}

Battleship::~Battleship(){

}

GrooveApp* Battleship::new_instance(GridTile *tile)
{
	return new Battleship(tile);
}

void Battleship::start() {
	tile->fillScreen(CRGB(0));
}

void Battleship::stop() {

}

void Battleship::onCommand(CommandMsg *msg)
{
	DynamicJsonDocument *doc = msg->doc;
	DynamicJsonDocument *rspdoc = msg->rspdoc;
	if(doc->containsKey("move"))
	{
		std::string move = (*doc)["move"].as<std::string>();
		if(move == "up")
		{
			if(crosshair.y > 1)
			{
				crosshair.y--;
			}
		}
		else if(move == "down")
		{
			if(crosshair.y < SIZE_Y*2)
			{
				crosshair.y++;
			}
		}
		else if(move == "left")
		{
			if(crosshair.x > 0)
			{
				crosshair.x--;
			}
		}
		else if(move == "right")
		{
			if(crosshair.x < (SIZE_X + SIZE_MIDBORDER)*2)
			{
				crosshair.x++;
			}
		}
	}
	else if(doc->containsKey("cmd"))
	{
		std::string cmd = (*doc)["cmd"].as<std::string>();
		if(cmd == "sendShip")
		{
			JsonObject shipObject = (*doc)["data"];
			if(shipObject.isNull())
			{
				(*rspdoc)["error"] = 1;
				return;
			}
			uint8_t shipID = (*doc)["id"].as<uint8_t>();
			Ship newShip = {shipID,shipObject["x"].as<uint8_t>(),shipObject["y"].as<uint8_t>(),shipObject["len"].as<uint8_t>(),shipObject["rot"].as<uint8_t>()};
			if(msg->channelID == CHANNEL_USER1)
			{
				shipList1.push_back(newShip);
				ESP_LOGI("Battleship","shiplist1 size: %i",shipList1.size());
			}
			else
			{
				shipList2.push_back(newShip);
				ESP_LOGI("Battleship","shiplist2 size: %i",shipList2.size());
			}
			ESP_LOGI("Battleship","ship: x=%i,y=%i,len=%i,rot=%i",newShip.x,newShip.y,newShip.len,newShip.rot);
		}
	}
}

void Battleship::drawAllShips(){
	std::list<Ship>::iterator it = shipList1.begin();
	while(it != shipList1.end())
	{
		Ship curship = *it;
		if(curship.rot == 0)	//along y axis
		{
			tile->writeFilledRect( (SIZE_X-curship.y-1)*2 , curship.x*2, 2, 2*curship.len, shipColor);
		}
		else	//along x axis
		{
			tile->writeFilledRect( (SIZE_X-curship.y-curship.len)*2 , curship.x*2, 2*curship.len, 2, shipColor);
		}
		it++;
	}
	it = shipList2.begin();
	while(it != shipList2.end())
	{
		Ship curship = *it;
		if(curship.rot == 0)	//along y axis
		{
			tile->writeFilledRect((curship.y+SIZE_MIDBORDER+SIZE_X)*2, (SIZE_Y - curship.x -curship.len)*2, 2, 2*curship.len, shipColor);
		}
		else	//along x axis
		{
			tile->writeFilledRect((curship.y+SIZE_MIDBORDER+SIZE_X)*2, (SIZE_Y - curship.x -1)*2, 2*curship.len, 2, shipColor);
		}
		it++;
	}
}

void Battleship::writePixel(uint8_t x, uint8_t y, CRGB color){
	tile->writeFilledRect(x*2,y*2,2,2,color);
}

void Battleship::drawWater()
{
	motionStep++;
	for(int i=0;i<tile->getHeight();i++)
	{
		colorIndex = i + motionStep;
		for(int j=tile->getWidth()-1;j>=0;j--)
		{
			tile->writePixel(j, i, ColorFromPalette(waterPalette, colorIndex, 255, LINEARBLEND));
			colorIndex+=1;
		}
	}
}

void Battleship::drawCrosshair(uint8_t x, uint8_t y)
{
	static uint8_t step = 0;
	if(crosshairTimer.isTimeUp())
	{
		step = (step+1)%6;
	}

	switch (step)
	{
	case 0:
		tile->writePixel(x,y+1,CRGB(0xFF0000));
		tile->writePixel(x,y,CRGB(0xFF0000));
		tile->writePixel(x+1,y,CRGB(0xFF0000));
		tile->writePixel(x+3,y+2,CRGB(0xFF0000));
		tile->writePixel(x+3,y+3,CRGB(0xFF0000));
		tile->writePixel(x+2,y+3,CRGB(0xFF0000));
		break;
	case 1:
		tile->writePixel(x,y,CRGB(0xFF0000));
		tile->writePixel(x,y+1,CRGB(0xFF0000));
		tile->writePixel(x,y+2,CRGB(0xFF0000));
		tile->writePixel(x+3,y+1,CRGB(0xFF0000));
		tile->writePixel(x+3,y+2,CRGB(0xFF0000));
		tile->writePixel(x+3,y+3,CRGB(0xFF0000));
		break;
	case 2:
		tile->writePixel(x,y+1,CRGB(0xFF0000));
		tile->writePixel(x,y+2,CRGB(0xFF0000));
		tile->writePixel(x,y+3,CRGB(0xFF0000));
		tile->writePixel(x+3,y,CRGB(0xFF0000));
		tile->writePixel(x+3,y+1,CRGB(0xFF0000));
		tile->writePixel(x+3,y+2,CRGB(0xFF0000));
		break;
	case 3:
		tile->writePixel(x,y+2,CRGB(0xFF0000));
		tile->writePixel(x,y+3,CRGB(0xFF0000));
		tile->writePixel(x+1,y+3,CRGB(0xFF0000));
		tile->writePixel(x+2,y,CRGB(0xFF0000));
		tile->writePixel(x+3,y,CRGB(0xFF0000));
		tile->writePixel(x+3,y+1,CRGB(0xFF0000));
		break;
	case 4:
		tile->writePixel(x+1,y,CRGB(0xFF0000));
		tile->writePixel(x+2,y,CRGB(0xFF0000));
		tile->writePixel(x+3,y,CRGB(0xFF0000));
		tile->writePixel(x,y+3,CRGB(0xFF0000));
		tile->writePixel(x+1,y+3,CRGB(0xFF0000));
		tile->writePixel(x+2,y+3,CRGB(0xFF0000));
		break;
	case 5:
		tile->writePixel(x,y,CRGB(0xFF0000));
		tile->writePixel(x+1,y,CRGB(0xFF0000));
		tile->writePixel(x+2,y,CRGB(0xFF0000));
		tile->writePixel(x+1,y+3,CRGB(0xFF0000));
		tile->writePixel(x+2,y+3,CRGB(0xFF0000));
		tile->writePixel(x+3,y+3,CRGB(0xFF0000));
		break;
	default:
		break;
	}
}

void Battleship::draw(){
	drawWater();
	switch (gameState)
	{
	case STATE_WAIT_FOR_SHIPS:
		if(shipList1.size() == NR_SHIPS || shipList2.size() == NR_SHIPS)
		{
			ESP_LOGI("Battleship","all ships there");
			gameState = STATE_CROSSHAIR;
		}
		break;
	case STATE_CROSSHAIR:
		tile->writeFilledRect(SIZE_X*2,0,2,SIZE_Y*2,CRGB(0));
		drawAllShips();
		drawCrosshair(crosshair.x,crosshair.y);
		break;
	case STATE_SHOOTING:

		break;
	}
	tile->endWrite();
}

void Battleship::reset(){
	tile->fillScreen(CRGB(0));
}


void Battleship::run() {
	if (frameTimer.isTimeUp()) {
		draw();
	}
}