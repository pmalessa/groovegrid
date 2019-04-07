
#include "PLATFORM.h"
#include "MainLoop.h"


void setup()
{
#if defined(__AVR__)
	srand(eeprom_read_word((uint16_t *)0x23));
	eeprom_update_word((uint16_t *)0x23, (uint16_t)rand());
#endif
}

// The loop function is called in an endless loop
void loop()
{
	static MainLoop& mainLoop = MainLoop::getInstance();
	mainLoop.loop();
}
