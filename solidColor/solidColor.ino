#include <FastLED.h>

#define NUM_LEDS 144
#define DATA_PIN 14

#define CHSV_COLOR 50, 100, 255

CRGB leds[NUM_LEDS];


void setup() { 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
    
  fill_solid(&leds[50], NUM_LEDS-70, CHSV(CHSV_COLOR));
  FastLED.show();
}

void loop() { 

}