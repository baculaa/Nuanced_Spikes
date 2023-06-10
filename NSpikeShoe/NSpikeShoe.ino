/// @file    NSpikeShoe.ino
/// @brief   Get data from an accelerometer and make a chain of LEDs on the shoe respond

#include <FastLED.h>
#include "pindefs.h"
#include "accelerometerCode.h"

// How many leds in your strip?
#define NUM_LEDS 1

// how many milliseconds between accelerometer reads
#define ACCELEROMETER_READ_PERIOD 20


//
// CHECK pindefs.h for what gets plugged in where!
//

// Define the array of leds
CRGB leds[NUM_LEDS];

// Array for storing running average values from the accelerometer
//  Adjust the size for faster reactivity, also related to 
//  how fast the accelerometer is set up to be read by ACCELEROMETER_READ_PERIOD
#define RUNNING_ARRAY_SIZE 256
uint16_t g_runningAvgArray [RUNNING_ARRAY_SIZE];
uint16_t g_runningIndex = 0;

void setup() { 
  #ifndef ESP8266
    while (!Serial) yield();     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(115200);
  Serial.println("Starting up...");

  setupAccelerometer();
  
  // initialize our running array
  for (int i = 0; i < RUNNING_ARRAY_SIZE; i++){
    g_runningAvgArray[i] = 0;
  }

  // Set up LEDs
     FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
 
}


//Global timers for keeping track of loop things
unsigned long g_timer_0 = 0;
// Cycle for LED blinking
#define CYCLE_TIME 100


void loop() { 

  // Read in accelerometer data
  uint32_t avg = 0;
  if (shouldWeReadAccelerometer(ACCELEROMETER_READ_PERIOD)){
    struct AccelerometerData aData = readAccelerometer();
    
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
  // Write LEDs
  uint8_t hue = avg / 4; // divided by 4, since we're currently using an analog read that goes 0-1024
  // Turn the LED on and off on a 1 second cycle
  if (g_timer_0 >= millis() + CYCLE_TIME / 2){
    leds[0]= CHSV( hue, 255, 50);
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
