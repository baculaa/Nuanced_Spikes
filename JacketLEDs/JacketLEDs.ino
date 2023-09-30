/// @file    JacketLEDs.ino
/// @brief   Get data from pot pin, distance from pi, light up leds, send pot data to mqtt

// GLOBAL VARIABLES
uint8_t gHue = 10;

#define FASTLED_INTERRUPT_RETRY_COUNT 100

#include <FastLED.h>
#include "pindefs.h"
#include "MQTTFunctions.h"

// How many leds in your strip?
#define NUM_LEDS 7

//Global timers for keeping track of loop things
unsigned long g_timer_0 = 0;

float duration,
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
  #ifndef ESP8266
    while (!Serial) yield();     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(115200);
  Serial.println("Starting up...");

  // Set up LEDs
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
 
  // Set up MQTT
  setupMQTT();
}


//Global timers for keeping track of loop things
//unsigned long g_timer_0 = 0;
// Cycle for LED blinking
//#define CYCLE_TIME 100


void loop() { 
  // Read analog pin data
  pot = analogRead(POT_PIN);
  // send the pot data over mqtt
  char buffer [5];
  itoa(pot, buffer, 10);
  mqttClient.publish("IROS/pot", 0, true, buffer);

  // Write LEDs
  ///uint8_t hue = avg / 4; // divided by 4, since we're currently using an analog read that goes 0-1024
//  delay(1000);
  uint8_t hue = gHue;
//  Serial.print ("gHue: ");
//  Serial.println(gHue);
//  Serial.print ("Hue: ");
//  Serial.println(hue);

  // Using hue instead of color, which we get from our mqtt topic! now we gotta set all the leds
  // and do what we wanna do with the stomp bit

  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
//      leds[whiteLed] = CRGB::Violet;

      // Show the leds (only one of which is set to white, from above)
      

      // Wait a little bit
      //delay(100);
        sat = 255;
        bright = 150;
        if (pot < 50) {
        // Turn our current led back to black for the next loop around
        color = 96;
//        Serial.println(color);
        bright2 = 100;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 900;
        }
        else if (pot < 100) {
        color = 64;
        bright2 = 75;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 700;
        }
        else if (pot < 150) {
        color = 40;
        bright2 = 50;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 500;
        }
        else if (pot < 200) {
        color = 20;
        bright2 = 25;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 300;
        }
        else{
          color = 0;
          bright2 = 0;
          leds[whiteLed] = CHSV(color, sat, bright);
          wait = 100;
        }
//        Serial.print("LED loop: ");
//      Serial.println(color);
      }
//  FastLED.show();
//  delay(wait);
//  // Turn the LED on and off on a 1 second cycle
//  if (g_timer_0 >= millis() + CYCLE_TIME / 2){
//    leds[0]= CHSV( hue, 255, 50);
//    FastLED.show();
//    digitalWrite(13, HIGH);
//  }
//  else if (g_timer_0 >= millis()){
//    digitalWrite(13, LOW);
//    leds[0]= CHSV( hue, 255, 50);
//    FastLED.show();
//  }
//  else {
//    g_timer_0 = millis() + CYCLE_TIME;
//  }

//      Serial.println(color);
//      Serial.println(wait);
      FastLED.show();
      delay(wait);

      // If a person is too close, then we flash
       if (hue < 99){
        
//          if (abs(distance-prev_dist) < 55){
//            Serial.print("Distance: ");
//            Serial.println(hue);
            for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
//              Serial.print("Flash loop: ");
//            Serial.println(whiteLed);
            leds[whiteLed] = CHSV(color, sat, bright2);
            } 
//        }
       }
      /// AND BLINK AT FIXED RATE FOR STOMP 
//        struct AccelerometerData aData = currentAccelData;
//       if (aData.z > 10){
// //        Serial.print("Here");
//         for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
//          bright = 255;
//          leds[whiteLed] = CHSV(color, sat, bright); 
//         }
//          wait = 100;
//       }
      FastLED.show();
      delay(wait);
//      Serial.print("Brightness: ");
//      Serial.println(bright);
     
   }

