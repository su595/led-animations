#include <FastLED.h>

#define NUM_LEDS 288
#define DATA_PIN 19
#define DELAY_BETWEEN_STEPS 3
#define DIMMING_SPEED 20
#define NO_DOTS 4

uint16_t dots[NO_DOTS];
int hues[NO_DOTS];

CRGB leds[NUM_LEDS];

void setup() { 
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.clear();

    // evenly space the dots to start with
    // evenly space hues
    // randomize starting hues
    for(int i = 0; i < NO_DOTS; i++){
        dots[i] = (2*NUM_LEDS/sizeof(dots)) * i;

        //hues[i] = random(0, 255);
        hues[i] = (2*255/sizeof(dots)) * i;
        //hues[i] = 0;
    }

}


void loop() { 

    // move each dot position by 1 led and check for overflow
    for(int i = 0; i < NO_DOTS; i++){
        // depending on the direction of the dot, subract or add one to the position
        // move dot to the other side of the strip and change hue
        dots[i]++;
        if(dots[i] > NUM_LEDS - 1){
            dots[i] = 0;
            changeHue(i);
        }
    }
    
    for(int i = 0; i < NO_DOTS; i++){
        leds[dots[i]].setHSV(hues[i], 255, 255);
    }

    //fadeall(DIMMING_SPEED);
    fadeall(random(0, DIMMING_SPEED));
    
    FastLED.show();

    delay(DELAY_BETWEEN_STEPS);  
}

void changeHue(int i){
    // Each time dot moves around the strip, the hue changes
    hues[i] = hues[i] + 40;
    // keep Hue within 0 and 255
    while(hues[i] > 255){
        hues[i] = hues[i] - 255;
    }
}


void fadeall(int8_t dimmingSpeed){
    if(dimmingSpeed < 0){
        for(int i = 0; i < NUM_LEDS; i++){
            leds[i] += leds[i].scale8(dimmingSpeed);
        }
    }
    if(dimmingSpeed > 0){
        for(int i = 0; i < NUM_LEDS; i++){
            leds[i].nscale8(256-dimmingSpeed);
        }
    }
}
