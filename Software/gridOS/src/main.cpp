
#include "PLATFORM.h"
#include "MainLoop.h"

extern "C" void app_main()
{
	const char* idleTaskTag = "IdleTask";
	delay(3000);
	static MainLoop& mainLoop = MainLoop::getInstance();
	UNUSED(mainLoop);
	ESP_LOGI(idleTaskTag,"Hey!\n");
	while(1)
	{
		ESP_LOGI(idleTaskTag,"Heap: %i",xPortGetFreeHeapSize());
		vTaskDelay(3000);
	}
}
