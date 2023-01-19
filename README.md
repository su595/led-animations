# led-animations
A collection of animations for the WS-8112B leds for the arduino.

All animation parameters can be adjusted at the start of a program, for example
```
vizualzed sort:
#define NUM_LEDS 288
#define DATA_PIN 19
// Determines the speed of the visual sorting
#define SORT_SPEED 50
// Determines the wait time after the leds have been newly randomized
#define START_DELAY 3000
#define END_DELAY 3000
// Default HSV value
#define HUE 60
#define SATURATION 255
#define VALUE 255
```
