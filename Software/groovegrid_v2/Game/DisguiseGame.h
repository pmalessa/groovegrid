
#include "../PLATFORM.h"

#include "../utils/GroovegridApp.h"
#include "../driver/Timer.h"

class DisguiseGame: public GroovegridApp{
public:
	DisguiseGame(GridTile *tile);
	~DisguiseGame();
	void reset();
	void run();
	void onInput(char *data);
private:
	uint32_t genFlashInterval();

	uint8_t gameState = 0;
	uint32_t flashTime = 0;
	uint32_t lastTime = 0;
	uint16_t fadePosition = 0;
	uint16_t fadeDelay = 100; //ms
	uint16_t flashDuration = 50; //ms
	uint32_t minFlashInterval = 100; //seconds
	uint32_t maxFlashInterval = 3000; //seconds
};
