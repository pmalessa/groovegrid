
#include "PLATFORM.h"
#include "MainLoop.h"


void setup()
{
	delay(3000);
	static MainLoop& mainLoop = MainLoop::getInstance();
	mainLoop.start();
}

// The loop function is called in an endless loop
void loop()
{
}
