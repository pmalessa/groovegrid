/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Grid.h"

CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];

void renderLoop(void *parameter)
{
	static Grid& grid = Grid::getInstance();
	Serial.print("renderTask started\n");
	while(true)
	{
		if(grid.renderTriggered == true)
		{
			grid.renderTriggered = false;
			FastLED.show();
		}
	}
}

extern "C" void createRenderTask(TaskHandle_t *renderTask)
{
	xTaskCreatePinnedToCore(renderLoop, /* Function to implement the task */
	      "renderTask", /* Name of the task */
	      10000,  /* Stack size in words */
	      NULL,  /* Task input parameter */
	      0,  /* Priority of the task */
	      renderTask,  /* Task handle. */
	      0); /* Core where the task should run */
}

Grid& Grid::getInstance()
{
	static Grid _instance;
	return _instance;
}

Grid::~Grid(){}
Grid::Grid()
{
#ifdef DOOR16
	FastLED.addLeds<NEOPIXEL,GRID_DATA_PIN>(matrixleds, GRID_WIDTH*GRID_HEIGHT);
#else
	FastLED.addLeds<NEOPIXEL,GRID_DATA1_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA2_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*1, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA3_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*2, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA4_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*3, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA5_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*4, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA6_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*5, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA7_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*6, NUM_LEDS_PER_CHANNEL);
#endif
	createRenderTask(&renderTask);
	renderTriggered = false;
}

void Grid::writePixel(int16_t x, int16_t y, uint16_t color)
{
	if(y%2 == 0)	//even row
	{
#ifdef DOOR16	//First LED Top Left
		matrixleds[GRID_WIDTH*y + x] = expandColor(color);
#else			//First LED Top Right
		matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = expandColor(color);
#endif
	}
	else			//odd row
	{
#ifdef DOOR16	//First LED Top Left
		matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = expandColor(color);
#else			//First LED Top Right
		matrixleds[GRID_WIDTH*y + x-1] = expandColor(color);
#endif
	}
}

void Grid::writePixel(int16_t x, int16_t y, CRGB color)
{
	if(y%2 == 0)	//even row
	{
#ifdef DOOR16	//First LED Top Left
		matrixleds[GRID_WIDTH*y + x] = color;
#else			//First LED Top Right
		matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
#endif
	}
	else			//odd row
	{
#ifdef DOOR16	//First LED Top Left
		matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
#else			//First LED Top Right
		matrixleds[GRID_WIDTH*y + x-1] = color;
#endif
	}
}

void Grid::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	writePixel(x, y, color);
	endWrite();
}

void Grid::endWrite()
{
	renderTriggered = true;
}

void Grid::clearDisplay()
{
	FastLED.clear(true);
}

// Downgrade 24-bit color to 16-bit
uint16_t Grid::RGB(uint8_t r, uint8_t g, uint8_t b)
{
	  return ((uint16_t)(r & 0xF8) << 8) |
	         ((uint16_t)(g & 0xFC) << 3) |
	                    (b         >> 3);
}
uint16_t Grid::RGB(uint32_t rgb)
{
	return RGB((rgb&0xFF0000)>>16,(rgb&0x00FF00)>>8,(rgb&0x0000FF)>>0);
}

uint16_t Grid::HSV(uint8_t h, uint8_t s, uint8_t v)
{
	UNUSED(h+s+v);

	//TODO
	return 0;
}

// Expand 16-bit input color (Adafruit_GFX colorspace) to 24-bit (NeoPixel)
// (w/gamma adjustment)
uint32_t Grid::expandColor(uint16_t color) {
  return ((uint32_t)pgm_read_byte(&gamma5[ color >> 11       ]) << 16) |
         ((uint32_t)pgm_read_byte(&gamma6[(color >> 5) & 0x3F]) <<  8) |
                    pgm_read_byte(&gamma5[ color       & 0x1F]);
}

void Grid::setBrightness(uint8_t brightness)
{
	FastLED.setBrightness(brightness);
}
