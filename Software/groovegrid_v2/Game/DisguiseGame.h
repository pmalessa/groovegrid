
#include "../PLATFORM.h"

#include "../utils/GroovegridApp.h"

class DisguiseGame: public GroovegridApp{
public:
	DisguiseGame();
	~DisguiseGame();
	void reset();
	void run();
	void onInput(char *data);
private:
	uint32_t genFlashInterval();

	uint8_t gameState = 0;
	uint32_t flashTime = 0;
	uint32_t lastFlashTime = 0;
	uint32_t minFlashInterval = 100; //seconds
	uint32_t maxFlashInterval = 3000; //seconds
};
