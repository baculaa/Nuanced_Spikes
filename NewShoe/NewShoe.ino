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
      FastLED.show();

      // Wait a little bit
      //delay(100);

        if (analogRead(POT_PIN) < 200) {
        // Turn our current led back to black for the next loop around
        leds[whiteLed] = CRGB::Green;
        wait = 900;
        }
        else if (analogRead(POT_PIN) < 400) {
        leds[whiteLed] = CRGB::Yellow;
        wait = 700;
        }
        else if (analogRead(POT_PIN) < 600) {
        leds[whiteLed] = CRGB::Orange;
        wait = 500;
        }
        else if (analogRead(POT_PIN) < 800) {
        leds[whiteLed] = CRGB::OrangeRed;
        wait = 300;
        }
        else{
          leds[whiteLed] = CRGB::Red;
          wait = 100;
        }
       
     
      }
      flag = 0;
      
       Serial.println(distance);
       if (distance < 50){
        for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
         leds[whiteLed] = CRGB::Black;
         flag = 1;
        }
         delay(wait);
      }

      
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
