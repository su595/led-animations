#include <FastLED.h>

#define NUM_LEDS 288
#define DATA_PIN 19
// Determines the speed of the visual sorting
#define SORT_SPEED 50
// Determines the wait time after the leds have been newly randomized
#define START_DELAY 3000
#define END_DELAY 3000
// Default HSV values when that is not the value sorted for
#define HUE 60
#define SATURATION 255
#define VALUE 255

// A global variable that is a list of our LEDs we can directly write CHSV objects to
CRGB leds[NUM_LEDS];

// making all variables global maybe
uint8_t work_list[NUM_LEDS];
uint8_t mode; // h, s or v
int pivot, value1, value2, temp;


void setup() {

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();

  Serial.begin(115200);
  Serial.println("test");

  mode = 0;
}

void loop() {
  Serial.println(mode);

  randomLeds();
  FastLED.show();
  delay(START_DELAY);

  quicksort_method(work_list, 0, NUM_LEDS - 1);
  
  //mode++;
  if(mode > 2) mode = 0;
  
  delay(END_DELAY);
}

void randomLeds() {
  // randomize the leds list (with random hues)

  Serial.println("All random values: ");
  for (int i = 0; i < NUM_LEDS; i++) {
    work_list[i] = random8();
    
    switch (mode){
      case 0:
        leds[i] = CHSV(work_list[i], SATURATION, VALUE);
        break;
      case 1:
        leds[i] = CHSV(HUE, work_list[i], VALUE);
        Serial.println("mode 1 here");
        break;
      case 2:
        leds[i] = CHSV(HUE, SATURATION, work_list[i]);
        break;
    }
    
    Serial.print(work_list[i]); Serial.print(", ");
  }
  Serial.println();

}

void quicksort_method(uint8_t element_list[], int low, int high)
{
  if (low < high)
  {
    pivot = low;
    value1 = low;
    value2 = high;
    while (value1 < value2)
    {
      while (element_list[value1] <= element_list[pivot] && value1 <= high)
      {
        value1++;
      }
      while (element_list[value2] > element_list[pivot] && value2 >= low)
      {
        value2--;
      }
      if (value1 < value2)
      {
        temp = element_list[value1];
        element_list[value1] = element_list[value2];
        element_list[value2] = temp;

        switch (mode){
          case 0:
            leds[value1] = CHSV(element_list[value1], SATURATION, VALUE);
            leds[value2] = CHSV(element_list[value2], SATURATION, VALUE);
            break;
          case 1:
            leds[value1] = CHSV(HUE, element_list[value1], VALUE);
            leds[value2] = CHSV(HUE, element_list[value2], VALUE);
            break;
          case 2:
            leds[value1] = CHSV(HUE, SATURATION, element_list[value1]);
            leds[value2] = CHSV(HUE, SATURATION, element_list[value2]);
            break;
        }
        FastLED.show();
        delay(SORT_SPEED);
      }
    }
    temp = element_list[value2];
    element_list[value2] = element_list[pivot];
    element_list[pivot] = temp;

    switch (mode){
      case 0:
        leds[value1] = CHSV(element_list[pivot], SATURATION, VALUE);
        leds[value2] = CHSV(element_list[value2], SATURATION, VALUE);
        break;
      case 1:
        leds[value1] = CHSV(HUE, element_list[pivot], VALUE);
        leds[value2] = CHSV(HUE, element_list[value2], VALUE);
        break;
      case 2:
        leds[value1] = CHSV(HUE, SATURATION, element_list[pivot]);
        leds[value2] = CHSV(HUE, SATURATION, element_list[value2]);
        break;
    }
    FastLED.show();
    delay(SORT_SPEED);

    quicksort_method(element_list, low, value2 - 1);
    quicksort_method(element_list, value2 + 1, high);
  }
}
