#include <FastLED.h>

#define NUM_LEDS 288
#define DATA_PIN 19
#define DELAY_BETWEEN_STEPS 3
#define DELAY_BETWEEN_COLORS 3

uint8_t Hue = 0;
uint8_t Saturation = 255;
uint8_t Value = 0;

CRGB leds[NUM_LEDS];

void setup() { 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
}


void loop() { 
  
  for(int i = 0; i < 255; i++){
    fill_solid(&leds[0], NUM_LEDS, CHSV(Hue, Saturation, Value));
    FastLED.show();
    //Saturation++;
    Value++;  
    delay(DELAY_BETWEEN_STEPS);
  }
  delay(DELAY_BETWEEN_COLORS);
  
  for(int i = 0; i < 255; i++){
    fill_solid(&leds[0], NUM_LEDS, CHSV(Hue, Saturation, Value));
    FastLED.show();
    //Saturation--;
    Value--;
    delay(DELAY_BETWEEN_STEPS);
  }
  delay(DELAY_BETWEEN_COLORS);

  //Hue wird als uint8_t automatisch bei 256 wieder zu 0
  Hue = Hue + 32;
  
}
