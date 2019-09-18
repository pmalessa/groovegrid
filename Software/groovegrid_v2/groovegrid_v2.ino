
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
	Serial.print("Heap: ");
	Serial.print(ESP.getFreeHeap());
	vTaskDelay(1000*MS_TO_TICKS);
}
