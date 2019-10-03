
#include "PLATFORM.h"
#include "MainLoop.h"


extern "C" void app_main()
{
	delay(3000);
	static MainLoop& mainLoop = MainLoop::getInstance();
	while(1)
	{
		mainLoop.loop();
	}
}
