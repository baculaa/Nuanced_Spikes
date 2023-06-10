/// @file    NSpikeShoe.ino
/// @brief   Get data from an accelerometer and make a chain of LEDs on the shoe respond

#include <FastLED.h>
#include "pindefs.h"
#include "accelerometerCode.h"

// How many leds in your strip?
#define NUM_LEDS 1


//
// CHECK pindefs.h for what gets plugged in where!
//

// Define the array of leds
CRGB leds[NUM_LEDS];


void setup() { 
  #ifndef ESP8266
    while (!Serial) yield();     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(115200);
  Serial.println("Starting up...");

  setupAccelerometer();
  
  // Set up LEDs
     FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
 
}


//Global timers for keeping track of loop things
unsigned long g_timer_0 = 0;
#define CYCLE_TIME 2000

void loop() { 

  // Read in accelerometer data
  struct AccelerometerData aData = readAccelerometer();
  
  Serial.print("X:  "); Serial.print(aData.x);
  Serial.print("  \tY:  "); Serial.print(aData.y);
  Serial.print("  \tZ:  "); Serial.println(aData.z);

  // Interpr;et accelerometer data

  // Write LEDs

  // Turn the LED on and off on a 1 second cycle
  if (g_timer_0 >= millis() + CYCLE_TIME / 2){
    leds[0] = 0x220022;
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
