
#include "PLATFORM.h"
#include "MainLoop.h"


void setup()
{
	delay(3000);
}

// The loop function is called in an endless loop
void loop()
{
	static MainLoop& mainLoop = MainLoop::getInstance();
	mainLoop.loop();
}
