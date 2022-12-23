#include <FastLED.h>

#define NUM_LEDS 288
#define DATA_PIN 19
// Determines the speed of the main loop
#define DELAY 2
// Determines the frequency of the drops (higher is less frequent)
#define RANDOM 30
// The speed at which the drops loose brightness (the larger the faster)
#define DIMMING_SPEED 3
#define DROP_RADIUS 8

#define MIN_H 130
#define MAX_H 190
#define S 200
#define V 255



// A global variable that is a list of our LEDs we can directly write CHSV objects to
CRGB leds[NUM_LEDS];


void setup() { 

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();

  Serial.begin(115200);
}

void loop(){ 
  // whether a new drop shall be created by testing if random numbers equal 0
  if(random(0, RANDOM) == 0){    
    createDrop(random(0, NUM_LEDS-1), DROP_RADIUS);
  }

  FastLED.show();
  fadeAll();     
  delay(DELAY);
}

void createDrop(int pos, int drop_radius){

  uint8_t hue = random(MIN_H, MAX_H);

  // the for loops will go through one more led than indicated by radius, but that will have a light intensity of 0
  
  // small tail: for every radius, light up an led more far away from pos with less intensity
  for(int i = 1; i <= drop_radius; i++){
    // check if position is within bounds of leds
    if( pos-i >= 0 && pos-i < NUM_LEDS ){
      float inten = float(drop_radius-i)/float(drop_radius);
      leds[pos-i].setHSV(hue, S, V*inten);
    }
  }
  
  // center
  leds[pos].setHSV(hue, S, V);

  // big tail
  for(int i = 1; i <= drop_radius; i++){
    // check if position is within bounds of leds
    if( pos+i >= 0 && pos+i < NUM_LEDS ){
      float inten = float(drop_radius-i)/float(drop_radius);
      leds[pos+i].setHSV(hue, S, V*inten);
    }
  }

  /*
  leds[pos - 3] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V * 0.3);
  leds[pos - 2] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V * 0.6);
  leds[pos - 1] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V * 0.8);
  leds[pos] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V);
  leds[pos + 1] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V * 0.8);
  leds[pos + 2] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V * 0.6);
  leds[pos + 3] = CHSV(CHSV_COLOR_H, CHSV_COLOR_S, CHSV_COLOR_V * 0.3);
  */
}

void fadeAll(){ 
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].nscale8(255-DIMMING_SPEED); 
  } 
}
