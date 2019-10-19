#include "Battleship.h"

Battleship::Battleship(GridTile *tile): GrooveGame(tile) {
	frameTimer.setTimeStep(FRAMERATE_MS);
	crosshairTimer.setTimeStep(150);
	gameState = STATE_WAIT_FOR_SHIPS;
	target.x = 3;
	target.y = 3;
	currentPlayerID = 1;

	for(uint8_t i=0;i<2;i++)
	{
		for(uint8_t j=0;j<GAMEFIELD_SIZE_BLOCKS_WIDTH;j++)
		{
			for(uint8_t k=0;k<GAMEFIELD_SIZE_BLOCKS_HEIGHT;k++)
			{
				waterShots[i][j][k] = 0;
			}		
		}
	}

	shootTimer.setTimeStep(5000);	//automatic shooting for debugging
}

Battleship::~Battleship(){
	shipList[0].clear();
	shipList[1].clear();
}

GrooveApp* Battleship::new_instance(GridTile *tile)
{
	return new Battleship(tile);
}

void Battleship::start() {
	ESP_LOGI("Battleship","start");
	tile->fillScreenBuffer(CRGB(0));
}

void Battleship::stop() {

}

void Battleship::onCommand(CommandMsg *msg)
{
	DynamicJsonDocument *doc = msg->doc;
	DynamicJsonDocument *rspdoc = msg->rspdoc;
	if(msg->channelID > CHANNEL_USER2)
	{
		ESP_LOGI("Battleship","only 2 Player Game!");
		(*rspdoc)["error"] = 2;
		return;
	}
	if(doc->containsKey("move"))
	{
		std::string move = (*doc)["move"].as<std::string>();
		moveCrosshair(move,msg->channelID);
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
			Ship newShip = {shipID,
				shipObject["x"].as<uint8_t>(),
				shipObject["y"].as<uint8_t>(),
				shipObject["len"].as<uint8_t>(),
				shipObject["rot"].as<uint8_t>(), 0};
			if(msg->channelID == CHANNEL_USER1)
			{
				shipList[0].push_back(newShip);
			}
			else
			{
				shipList[1].push_back(newShip);
			}
			ESP_LOGI("Battleship","ship: x=%i,y=%i,len=%i,rot=%i",newShip.x,newShip.y,newShip.len,newShip.rot);
		}
		else if (cmd == "shoot")
		{
			if(shoot() == true){ //if hit
				(*msg->rspdoc)["hit"] = 1;
				ESP_LOGI("Battleship","shoot hit true");
			}
			else
			{
				(*msg->rspdoc)["hit"] = 0;
				ESP_LOGI("Battleship","shoot hit false");
			}
			(*msg->rspdoc)["delay"] = HITANIMATION_DELAY;
		}
		else
		{
			(*rspdoc)["error"] = 3;
		}
	}
	else
	{
		(*rspdoc)["error"] = 4;
	}
}

bool Battleship::isShipThere(uint8_t block_x, uint8_t block_y, uint8_t playerID)
{
	std::list<Ship>::iterator it = shipList[playerID-1].begin();
	while(it != shipList[playerID-1].end())
	{
		Ship curship = *it;
		for(uint8_t i=0;i<curship.len;i++)
		{
			if(curship.rot == 0 && (curship.x+i) == block_x && curship.y == block_y)	//along x axis
			{
				ESP_LOGI("Battleship","x=%i, y=%i,isShipThere true",block_x,block_y);
				return true;
			}else if(curship.rot == 1 && curship.x == block_x && (curship.y+1) == block_y)	//along y axis
			{
				ESP_LOGI("Battleship","x=%i, y=%i,isShipThere true",block_x,block_y);
				return true;
			}
		}
		it++;
	}
	ESP_LOGI("Battleship","x=%i, y=%i,isShipThere false",block_x,block_y);
	return false;
}

bool Battleship::shoot()
{
	if(isShipThere(target.x,target.y,currentPlayerID) == 1)
	{
		target.hit = 1;
		ESP_LOGI("Battleship","->STATE_SHOOTING");
		gameState = STATE_SHOOTING;
		return true;
	}
	else
	{
		target.hit = 0;
		ESP_LOGI("Battleship","->STATE_SHOOTING");
		gameState = STATE_SHOOTING;
		return false;
	}
}

void Battleship::setHitmap()
{
	std::list<Ship>::iterator it = shipList[currentPlayerID-1].begin();
	while(it != shipList[currentPlayerID-1].end())
	{
		Ship curship = *it;
		for(uint8_t i=0;i<curship.len;i++)
		{
			if(curship.rot == 0 && (curship.x+i) == target.x && curship.y == target.y)	//along x axis
			{
				ESP_LOGI("Battleship","target x%i,y%i,setHitmap pos %i",target.x,target.y,i);
				(*it).hitmap |= (1<<i);
				return;
			}else if(curship.rot == 1 && curship.x == target.x && (curship.y+i) == target.y)	//along y axis
			{
				ESP_LOGI("Battleship","target x%i,y%i,setHitmap pos %i",target.x,target.y,i);
				(*it).hitmap |= (1<<i);
				return;
			}
		}
		it++;
	}
	ESP_LOGI("Battleship","no ship hit");//no ship hit
	waterShots[currentPlayerID-1][target.x][target.y] = 1;
}

void Battleship::drawAllShips(){
	std::list<Ship>::iterator it;
	for(uint8_t j=1;j<=2;j++) //1..2
	{
		it = shipList[j-1].begin();
		while(it != shipList[j-1].end())
		{
			Ship curship = *it;
			if(curship.rot == 0)	//along playergrid x axis
			{
				for(uint8_t i=0;i<curship.len;i++)
				{
					writeBlock(curship.x+i,curship.y,j,shipColor);
				}
			}
			else	//along playergrid y axis
			{
				for(uint8_t i=0;i<curship.len;i++)
				{
					writeBlock(curship.x,curship.y+i,j,shipColor);
				}
			}
			it++;
		}
	}
}

void Battleship::moveCrosshair(std::string cmd, uint8_t playerID)
{
	if(playerID != currentPlayerID && gameState == STATE_CROSSHAIR)
	{
		shootTimer.reset();
		if(cmd == "down" && target.y > 0)
		{
			ESP_LOGI("Battleship","move Crosshair %s",cmd.c_str());
			target.y--;
		}
		if(cmd == "up" && target.y < GAMEFIELD_SIZE_BLOCKS_HEIGHT-1)
		{
			ESP_LOGI("Battleship","move Crosshair %s",cmd.c_str());
			target.y++;
		}
		if(cmd == "right" && target.x > 0)
		{
			ESP_LOGI("Battleship","move Crosshair %s",cmd.c_str());
			target.x--;
		}
		if(cmd == "left" && target.x < GAMEFIELD_SIZE_BLOCKS_WIDTH-1)
		{
			ESP_LOGI("Battleship","move Crosshair %s",cmd.c_str());
			target.x++;
		}
	}
}

//transform block coordinate from player coordinates to grid pixel coordinates
void Battleship::writeBlock(uint8_t block_x, uint8_t block_y, uint8_t playerID, CRGB color){
	if(playerID == 1){ //Player 1, left side, coord origin at (GAMEFIELD_SIZE_PIXEL_HEIGHT-1 , 0)
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT-2) - block_y*2,  block_x*2,  color);
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT-1) - block_y*2,  block_x*2,  color);
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT-2) - block_y*2,  block_x*2+1,  color);
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT-1) - block_y*2,  block_x*2+1,  color);
	}else if(playerID == 2){ //Player 2, right side, coord origin at (GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER -1 , GAMEFIELD_SIZE_PIXEL_WIDTH -1 )
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER) + block_y*2,  GAMEFIELD_SIZE_PIXEL_WIDTH-1 - block_x*2,  color);
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER+1) + block_y*2,  GAMEFIELD_SIZE_PIXEL_WIDTH-1 - block_x*2,  color);
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER) + block_y*2,  GAMEFIELD_SIZE_PIXEL_WIDTH-2 - block_x*2,  color);
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER+1) + block_y*2,  GAMEFIELD_SIZE_PIXEL_WIDTH-2 - block_x*2,  color);
	}
}

//transform pixel coordinate from player coordinates to grid coordinates
void Battleship::writePixel(uint8_t pixel_x, uint8_t pixel_y, uint8_t playerID, CRGB color){
	if(playerID == 1){ //Player 1, left side, coord origin at (GAMEFIELD_SIZE_PIXEL_HEIGHT-1 , 0)
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT-1) - pixel_y,  pixel_x,  color);
	}else if(playerID == 2){ //Player 2, right side, coord origin at (GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER -1 , GAMEFIELD_SIZE_PIXEL_WIDTH -1 )
		tile->writePixel((GAMEFIELD_SIZE_PIXEL_HEIGHT + GAMEFIELD_SIZE_PIXEL_MIDBORDER) + pixel_y,  GAMEFIELD_SIZE_PIXEL_WIDTH-1 - pixel_x,  color);
	}
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

void Battleship::drawHitmap()
{
	std::list<Ship>::iterator it;
	for(uint8_t j=0;j<2;j++)
	{
		it = shipList[j].begin();
		while(it != shipList[j].end())
		{
			Ship curship = *it;
			for(uint8_t i=0;i<curship.len;i++)
			{
				if(curship.hitmap & (1<<i))
				{
					if(curship.rot == 0)
					{
						writeBlock(curship.x+i,curship.y,j+1,hitColor);
					}
					else
					{
						writeBlock(curship.x,curship.y+i,j+1,hitColor);
					}
						
				}
			}
			it++;
		}
		for(uint8_t i=0;i<GAMEFIELD_SIZE_BLOCKS_WIDTH;i++)
		{
			for(uint8_t k=0;k<GAMEFIELD_SIZE_BLOCKS_HEIGHT;k++)
			{
				if(waterShots[j][i][k] == 1)
				{
					writeBlock(i,k,j+1,waterShotColor);
				}
			}
		}
	}
}

void Battleship::drawCrosshair(uint8_t block_x, uint8_t block_y, uint8_t playerID)
{
	static uint8_t step = 0;
	block_x = block_x*2 -1;
	block_y = block_y*2 -1;
	if(crosshairTimer.isTimeUp())
	{
		step = (step+1)%6;
	}

	switch (step)
	{
	case 0:
		writePixel(block_x,		block_y+1,	playerID, crosshairColor);
		writePixel(block_x,		block_y,	playerID, crosshairColor);
		writePixel(block_x+1,	block_y,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+2,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+3,	playerID, crosshairColor);
		writePixel(block_x+2,	block_y+3,	playerID, crosshairColor);
		break;
	case 1:
		writePixel(block_x,		block_y,	playerID, crosshairColor);
		writePixel(block_x,		block_y+1,	playerID, crosshairColor);
		writePixel(block_x,		block_y+2,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+1,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+2,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+3,	playerID, crosshairColor);
		break;
	case 2:
		writePixel(block_x,		block_y+1,	playerID, crosshairColor);
		writePixel(block_x,		block_y+2,	playerID, crosshairColor);
		writePixel(block_x,		block_y+3,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+1,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+2,	playerID, crosshairColor);
		break;
	case 3:
		writePixel(block_x,		block_y+2,	playerID, crosshairColor);
		writePixel(block_x,		block_y+3,	playerID, crosshairColor);
		writePixel(block_x+1,	block_y+3,	playerID, crosshairColor);
		writePixel(block_x+2,	block_y,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+1,	playerID, crosshairColor);
		break;
	case 4:
		writePixel(block_x+1,	block_y,	playerID, crosshairColor);
		writePixel(block_x+2,	block_y,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y,	playerID, crosshairColor);
		writePixel(block_x,		block_y+3,	playerID, crosshairColor);
		writePixel(block_x+1,	block_y+3,	playerID, crosshairColor);
		writePixel(block_x+2,	block_y+3,	playerID, crosshairColor);
		break;
	case 5:
		writePixel(block_x,		block_y,	playerID, crosshairColor);
		writePixel(block_x+1,	block_y,	playerID, crosshairColor);
		writePixel(block_x+2,	block_y,	playerID, crosshairColor);
		writePixel(block_x+1,	block_y+3,	playerID, crosshairColor);
		writePixel(block_x+2,	block_y+3,	playerID, crosshairColor);
		writePixel(block_x+3,	block_y+3,	playerID, crosshairColor);
		break;
	}
}

void Battleship::switchPlayer()
{
	if(currentPlayerID == 1)
	{
		currentPlayerID = 2;
	}
	else
	{
		currentPlayerID = 1;
	}
	ESP_LOGI("Battleship","switchPlayer -> P%i",currentPlayerID);
}

void Battleship::drawMidBorder()
{
	tile->writeFilledRect(GAMEFIELD_SIZE_PIXEL_HEIGHT,0,GAMEFIELD_SIZE_PIXEL_MIDBORDER,GAMEFIELD_SIZE_PIXEL_WIDTH-1,CRGB(0));
}

bool Battleship::shootAnimation()
{
	static uint8_t init = 1;
	if(init == 1)
	{
		init = 0;
		cannonBall.x = 0;
		cannonBall.y = 7;
		cannonBall.h = 0;
		cannonBall.vel = 10;
	}
	return true;
}

void Battleship::draw(){
	drawWater();
	switch (gameState)
	{
	case STATE_WAIT_FOR_SHIPS:
		if(shipList[0].size() == NR_SHIPS && shipList[1].size() == NR_SHIPS)
		{
			ESP_LOGI("Battleship","all ships there, ->STATE_CROSSHAIR");
			gameState = STATE_CROSSHAIR;
			shootTimer.reset();
		}
		break;
	case STATE_CROSSHAIR:
		drawMidBorder();
		//drawAllShips();
		drawHitmap();
		drawCrosshair(target.x,target.y, currentPlayerID);
		if(shootTimer.isTimeUp())
		{
			shoot();
			std::list<Ship>::iterator it;
			for(uint8_t j=0;j<2;j++)
			{
				it = shipList[j].begin();
				while(it != shipList[j].end())
				{
					ESP_LOGI("Battleship","hitmap P%i ship: %x",j+1,(*it).hitmap);
					it++;
				}
			}
		}
		break;
	case STATE_SHOOTING:
		drawMidBorder();
		drawHitmap();
		if(shootAnimation() == true)	//if shootAnimation done
		{
			setHitmap();
			gameState = STATE_CROSSHAIR;
			ESP_LOGI("Battleship","-> STATE_CROSSHAIR");
			switchPlayer();
		}
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