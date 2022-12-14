#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>

#include "ANIMATION.h"

//The setup function is called once at startup of the sketch
#define XMAX 4
#define YMAX 4
#define DATA_PIN 6

//4x4 display, starting at top left and going down zigzag
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(XMAX, YMAX, DATA_PIN,
		  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
		  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
		  NEO_GRB            + NEO_KHZ800);

void setup()
{
	Timer1.initialize();
	matrix.begin();
	Serial.begin(9600);
	Serial.println("Hey!");

	for (uint8_t i=0; i < YMAX; i++) {
		matrix.drawLine(0, i, XMAX-1, i, matrix.Color(255, 0, 0));
		matrix.show();
		delay(100);
	}
	for (uint8_t i=0; i < YMAX; i++) {
		matrix.drawLine(0, i, XMAX-1, i, matrix.Color(0, 0, 0));
		matrix.show();
		delay(100);
	}
	Timer1.attachInterrupt(timer, 1000);
}

// The loop function is called in an endless loop
void loop()
{
}

void timer()	//called every 1ms
{
	static uint32_t wait = 30000;
	static uint8_t state = 0;
	if(wait == 0)
	{
		state++;	//switch Animation every 10 seconds
		matrix.clear();
		wait = 30000;
	}
	wait--;
	switch (state) {
		case 0:
			ANIMATION_randomPixels(&matrix);
			break;
		case 1:
			ANIMATION_randomLines(&matrix);
			break;
		default:
			state = 0;
			break;
	}
	matrix.show();
}
