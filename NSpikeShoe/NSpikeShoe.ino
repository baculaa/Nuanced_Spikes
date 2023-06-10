/// @file    NSpikeShoe.ino
/// @brief   Get data from an accelerometer and make a chain of LEDs on the shoe respond

#include <FastLED.h>
#include "pindefs.h"

// How many leds in your strip?
#define NUM_LEDS 1


//
// CHECK pindefs.h for what gets plugged in where!
//

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  
  // Set uop
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

    // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

    // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
     FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
 
}


//Global timers for keeping track of loop things
unsigned long g_timer_0 = 0;
#define CYCLE_TIME 2000

void loop() { 

  // Read in accelerometer data

  // Interpret accelerometer data

  // Write LEDs

  // Turn the LED on and off on a 1 second cycle
  if (g_timer_0 >= millis() + CYCLE_TIME / 2){
    leds[0] = CRGB::Red;
    FastLED.show();
    digitalWrite(13, HIGH);
  }
  else if (g_timer_0 >= millis()){
    digitalWrite(13, LOW);
    leds[0] = CRGB::Black;
    FastLED.show();
  }
  else {
    g_timer_0 = millis() + CYCLE_TIME;
  }
}
