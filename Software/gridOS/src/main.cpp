
#include "PLATFORM.h"
#include "MainLoop.h"
#include "driver/DeltaTimer.h"

extern "C" void app_main()
{
	const char* idleTaskTag = "IdleTask";
	delay(1000);
	MainLoop::getInstance();
	ESP_LOGI(idleTaskTag,"Hey!\n");

	while(1)
	{
		ESP_LOGI(idleTaskTag,"Heap: %i",xPortGetFreeHeapSize());
		vTaskDelay(5000);
	}
}
