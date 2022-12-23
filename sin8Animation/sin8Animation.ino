#include <FastLED.h>

#define NUM_LEDS 144
#define DATA_PIN 3
#define DELAY_BETWEEN_STEPS 20

uint8_t Hue = 0;
uint8_t Saturation = 255;
uint8_t Value = 0;

CRGB leds[NUM_LEDS];

void setup() { 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  Serial.begin(9600);
}


void loop() { 
  
  for(int i = 0; i < 255; i++){
    Value = sin8(i);  
    fill_solid(&leds[0], 144, CHSV(Hue, Saturation, Value));
    FastLED.show();
    delay(DELAY_BETWEEN_STEPS);
  }
  
  Hue =+ 16;
}
