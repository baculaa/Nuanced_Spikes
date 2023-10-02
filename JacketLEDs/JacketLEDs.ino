/// @file    JacketLEDs.ino
/// @brief   Get data from pot pin, distance from pi, light up leds, send pot data to mqtt

#define FASTLED_INTERRUPT_RETRY_COUNT 100

#include <FastLED.h>
#include "pindefs.h"
#include "UDPFunctions.h"

// How many leds in your strip?
#define NUM_LEDS 16

int wait = 100;
int flag;
int color;
int sat;
int bright;
int bright2;
int pot = 30;
// Cycle for LED blinking
#define CYCLE_TIME 100

//
// CHECK pindefs.h for what gets plugged in where!
//

// Define the array of leds
CRGB leds[NUM_LEDS];



void setup() { 
//  #ifndef ESP8266
//    while (!Serial) yield();     // will pause Zero, Leonardo, etc until serial console opens
//  #endif
//
//  Serial.begin(115200);
//  Serial.println("Starting up...");

  // Set up LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
 
  // Set up MQTT
  udpSetup();
}


//Global timers for keeping track of loop things
//unsigned long g_timer_0 = 0;
// Cycle for LED blinking
//#define CYCLE_TIME 100


void loop() { 
  // Read analog pin data
  pot = analogRead(POT_PIN);

  sat = 255;
  bright = 150;
  if (pot < 200) {
    // Send UDP for Pot == 0
    sendPotentiometerData('0');
    color = 96;
    bright2 = 100;
    wait = 900;
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CHSV(color, sat, bright);
    }
  } else if (pot < 400) {
    // Send UDP for Pot == 1
    sendPotentiometerData('1');
    color = 64;
    bright2 = 75;
    wait = 700;
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    leds[whiteLed] = CHSV(color, sat, bright);
    }
  } else if (pot < 600) {
    // Send UDP for Pot == 2
    sendPotentiometerData('2');
    color = 40;
    bright2 = 50;
    wait = 500;
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CHSV(color, sat, bright);
    }
  }
  else if (pot < 800) {
    // Send UDP for Pot == 3
    sendPotentiometerData('3');
    color = 20;
    bright2 = 25;
    wait = 300;
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CHSV(color, sat, bright);
    }
  }
  else{
    // Send UDP for Pot == 4
    sendPotentiometerData('4');
    color = 0;
    bright2 = 0;
    wait = 100;
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      leds[whiteLed] = CHSV(color, sat, bright);
    }
  }
  // Serial.print("LED loop: ");
  // Serial.println(color);
  // Serial.println(wait);
  FastLED.show();
  delay(wait);

  // If a person is too close, then we flash
   if (distance == '1'){
        for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
        leds[whiteLed] = CHSV(color, sat, bright2);
        }
        FastLED.show();
   delay(wait); 
   }
   
  // Serial.print("Brightness: ");
  // Serial.println(bright);  
}
