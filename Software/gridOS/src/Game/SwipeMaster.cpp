#include "SwipeMaster.h"

SwipeMaster::SwipeMaster(GridTile *tile): GrooveGame(tile) {
	frameTimer.setTimeStep(FRAMERATE_MS);
	middle = tile->getWidth()/2;
	position = middle;
}

SwipeMaster::~SwipeMaster(){

}

GrooveApp* SwipeMaster::new_instance(GridTile *tile)
{
	return new SwipeMaster(tile);
}

void SwipeMaster::start() {
	tile->fillScreen(CRGB(0));
}

void SwipeMaster::stop() {

}

void SwipeMaster::onCommand(CommandMsg *msg)
{
	std::string move = (*msg->doc)["move"].as<std::string>();
	
	switch(msg->channelID)
	{
		case CHANNEL_USER1: //++
			if(move=="up")
			{
				position++;
				ESP_LOGI("SwipeMaster","pos: %i",position);
			}
			break;
		case CHANNEL_USER2: //--
			if(move=="up")
			{
				position--;
				ESP_LOGI("SwipeMaster","pos: %i",position);
			}
			break;
	}
}

void SwipeMaster::draw(){
	if(position < middle)
	{
		tile->writeFilledRect(position,0,middle-position,tile->getHeight(),CRGB(0xFF0000));
	}
	else if(position > middle)
	{
		tile->writeFilledRect(middle,0,position-middle,tile->getHeight(),CRGB(0x0000FF));
	}
	tile->writeLine(middle,0,middle,tile->getHeight(),CRGB(0x00FF00));	//middle line
	tile->endWrite();
}

void SwipeMaster::reset(){
	position = middle; //reset
	tile->fillScreen(CRGB(0));
}

void SwipeMaster::checkWin(){
	if(position < 0)
	{
		tile->writeFilledRect(0,0,tile->getWidth(),tile->getHeight(),CRGB(0xFF0000));//player 2 wins
		tile->endWrite();
		ESP_LOGI("SwipeMaster","P2 Wins");
		vTaskDelay(3000);
		reset();
	}
	else if(position > tile->getWidth())
	{
		tile->writeFilledRect(0,0,tile->getWidth(),tile->getHeight(),CRGB(0x0000FF));//player 1 wins
		tile->endWrite();
		ESP_LOGI("SwipeMaster","P1 Wins");
		vTaskDelay(3000);
		reset();
	}
}

void SwipeMaster::run() {
	if (frameTimer.isTimeUp()) {
		checkWin();
		draw();
	}
}