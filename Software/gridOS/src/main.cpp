
#include "PLATFORM.h"
#include "MainLoop.h"
#include "driver/DeltaTimer.h"
#include "driver/Storage.h"

extern "C" void app_main()
{
	Storage::init();
	const char* idleTaskTag = "IdleTask";
	delay(1000);
	uint32_t brightness = Storage::getConfig("Brightness");
	MainLoop::start();
	ESP_LOGI(idleTaskTag,"Hey! %i\n",brightness);

	while(1)
	{
		ESP_LOGI(idleTaskTag,"Heap: %i",xPortGetFreeHeapSize());
		vTaskDelay(5000);
	}
}
