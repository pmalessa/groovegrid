
#include "../PLATFORM.h"
#include "../utils/GrooveGame.h"

class DisguiseGame: public GrooveGame{
public:
	DisguiseGame(GridTile *tile);
	~DisguiseGame();
	void run();
	void start();
	void stop();
    std::string onUserRead(uint8_t channelID);
    void onUserWrite(std::string data, uint8_t channelID);
private:
	uint32_t genFlashInterval();

	uint8_t gameState = 0;
	uint16_t fadePosition = 0;
	uint16_t fadeDuration = 1000; //ms
	uint16_t flashDuration = 50; //ms
	uint32_t minFlashInterval = 100; //seconds
	uint32_t maxFlashInterval = 3000; //seconds
};
