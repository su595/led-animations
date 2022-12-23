#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 144

// from 255 how fast the leds fade
#define FADE_SPEED 250 
#define DATA_PIN 7


// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(115200);
	Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(84);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(FADE_SPEED); } }

void loop() { 
  

  moveIt(random16());

  delay(200);
  
}

void moveIt(CRGB newColor){
  for(int i = NUM_LEDS; i <= 0; i--){
    // for the first led, give it a new color
    if(i == 0){
      leds[i] = newColor;
    }
    // set the next led to the last leds color
    leds[i] = leds[i-1];
  }
}

void funseed(){
  // micros() is to a precision of 8us (for a 8Mhz Arduino), that means the first three bits will be irrelevant
  uint32_t a = micros();
  uint32_t b = millis();
  uint16_t c = 0;

  for(int i = 0; i < 8; i++){
    // take 4 bits of a at a time
    uint8_t x = (a >> i*4) & 0x0F;

    // if x is even, add one to c
    if(x % 2 == 0){
      c += 1;
    }
    // and shift c by one
    c = c << 1;
  }
  
  for(int i = 0; i < 8; i++){
    // take 4 bits of b at a time
    uint8_t x = (b >> i*4) & 0x0F;

    // if x is odd, add one to c
    if(x % 2 != 0){
      c += 1;
    }
    // and shift c by one
    c = c << 1;
  }

  // now, c is as unpredictable as I will make it, and I use it as a seed for a random number generator
  random16_set_seed (c);
  // some entropy for fun (don't have anything connected to A0)
  random16_add_entropy (analogRead(A0));
}
