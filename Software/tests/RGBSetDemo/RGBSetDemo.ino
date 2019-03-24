#include <FastLED.h>
#define NUM_LEDS 88

CRGBArray<NUM_LEDS> leds;

void setup() { FastLED.addLeds<NEOPIXEL,32>(leds, NUM_LEDS); 

  Serial.begin(115200);
  Serial.println("Hey!");

  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out

    // let's set an led value
    leds[i] = CHSV(0,255,127);

    // now, let's first 20 leds to the top 20 leds, 
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(5);
  }
}

void loop(){ 
  /*
  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);

    // let's set an led value
    leds[i] = CHSV(hue++,255,255);

    // now, let's first 20 leds to the top 20 leds, 
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(5);
  }
  Serial.println(".");
  */
}
