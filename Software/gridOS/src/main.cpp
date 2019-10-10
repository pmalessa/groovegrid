
#include "PLATFORM.h"
#include "MainLoop.h"


extern "C" void app_main()
{
	delay(3000);
	static MainLoop& mainLoop = MainLoop::getInstance();
	UNUSED(mainLoop);
	while(1)
	{
		//Idle Task, print running tasks here
		vTaskDelay(3000);
	}
}
