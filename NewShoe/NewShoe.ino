/// @file    NewShoe.ino
/// @brief   Get data from an accelerometer and make a chain of LEDs on the shoe respond

#include <FastLED.h>
#include "pindefs.h"
#include "accelerometerCode.h"

#define NUM_LEDS 32

// how many milliseconds between accelerometer reads
#define ACCELEROMETER_READ_PERIOD 20
//Global timers for keeping track of loop things
unsigned long g_timer_0 = 0;
// Cycle for LED blinking
#define CYCLE_TIME 100

//const int trigPin = 9;
//const int echoPin = 10;

float duration, distance;
int wait;
int flag;
int color;
int sat;
int bright;
int bright2;
float prev_dist;

CRGB leds[NUM_LEDS];

// Array for storing running average values from the accelerometer
//  Adjust the size for faster reactivity, also related to 
//  how fast the accelerometer is set up to be read by ACCELEROMETER_READ_PERIOD
#define RUNNING_ARRAY_SIZE 256
uint16_t g_runningAvgArray [RUNNING_ARRAY_SIZE];
uint16_t g_runningIndex = 0;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
   #ifndef ESP8266
    while (!Serial) yield();     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(9600);
  Serial.println("Starting up...");
  prev_dist = 0;
  setupAccelerometer();

  // initialize our running array
  for (int i = 0; i < RUNNING_ARRAY_SIZE; i++){
    g_runningAvgArray[i] = 0;
  }

  //delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  // Read in accelerometer data
  uint32_t avg = 0;
  if (haveNewAccelData())
  {
  //if (shouldWeReadAccelerometer(ACCELEROMETER_READ_PERIOD)){
    //struct AccelerometerData aData = readAccelerometer();
    struct AccelerometerData aData = currentAccelData;
    
    Serial.print("X:  "); Serial.print(aData.x);
    Serial.print("  \tY:  "); Serial.print(aData.y);
    Serial.print("  \tZ:  "); Serial.println(aData.z);

    // Interpret accelerometer data
    // adding data to running array
    g_runningAvgArray[g_runningIndex] = aData.z;
    g_runningIndex++;
    if (g_runningIndex >= RUNNING_ARRAY_SIZE){
      g_runningIndex = 0;
    }
    uint32_t sum = 0;
    for (int i = 0; i < RUNNING_ARRAY_SIZE; i++){
      sum += g_runningAvgArray[i];
    }
    avg = sum / RUNNING_ARRAY_SIZE;
    Serial.print("Average: ");
    Serial.println(avg);

  }

  // Move a single white led 

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration*.0343)/2;
  
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
//      leds[whiteLed] = CRGB::Violet;

      // Show the leds (only one of which is set to white, from above)
      

      // Wait a little bit
      //delay(100);
        sat = 255;
        bright = 150;
        if (analogRead(POT_PIN) < 200) {
        // Turn our current led back to black for the next loop around
        color = 96;
        bright2 = 100;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 900;
        }
        else if (analogRead(POT_PIN) < 400) {
        color = 64;
        bright2 = 75;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 700;
        }
        else if (analogRead(POT_PIN) < 600) {
        color = 40;
        bright2 = 50;
        leds[whiteLed] = CHSV(color, sat, bright);
        wait = 500;
        }
        else if (analogRead(POT_PIN) < 800) {
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
      }
      FastLED.show();
      delay(wait);
      flag = 0;
      /// DESIRED OUTPUT: BLINK AT VARIABLE RATE FOR PROXIMITY DETECTIION
      struct AccelerometerData aData = currentAccelData;

       if (distance < 50){
        if (distance > 15){
//          if (abs(distance-prev_dist) < 55){
            Serial.print("Distance dif: ");
            Serial.println(distance);
            for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
            leds[whiteLed] = CHSV(color, sat, bright2);
            } 
        }
//        }
       }
      /// AND BLINK AT FIXED RATE FOR STOMP 
      if (aData.z > 10){
        Serial.print("Here");
        for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
         bright = 255;
         leds[whiteLed] = CHSV(color, sat, bright); 
        }
         wait = 100;
      }
      FastLED.show();
      delay(wait);
      Serial.print("Brightness: ");
      Serial.println(bright);
      prev_dist = distance;
      
   }
//}

/*
unsigned long g_accel_timer = 0;
bool shouldWeReadAccelerometer(long readPeriod){
  if (g_accel_timer < millis()){
    g_accel_timer = millis() + readPeriod;
    return true;
  }
  else{
    return false;
  }
}
*/
