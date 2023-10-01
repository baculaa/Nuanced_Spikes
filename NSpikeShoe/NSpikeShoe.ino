/// @file    NSpikeShoe.ino
/// @brief   Get data from an accelerometer and make a chain of LEDs on the shoe respond

// GLOBAL VARIABLES
//uint8_t gHue = 10;
//uint8_t gpot = 10;

// #define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 100

#include <FastLED.h>
#include "pindefs.h"
#include "accelerometerCode.h"
//#include "MQTTFunctions.h"
#include "UDPFunctions.h"

// How many leds in your strip?
#define NUM_LEDS 32

// how many milliseconds between accelerometer reads
#define ACCELEROMETER_READ_PERIOD 20
//Global timers for keeping track of loop things
unsigned long g_timer_0 = 0;

float duration; //, distance;
int wait = 1000;
int flag;
int color;
int sat;
int bright;
int bright2;
//float prev_dist;

// Cycle for LED blinking
#define CYCLE_TIME 100



//
// CHECK pindefs.h for what gets plugged in where!
//

// Define the array of leds
CRGB leds[NUM_LEDS];

// Array for storing running average values from the accelerometer
//  Adjust the size for faster reactivity, also related to
//  how fast the accelerometer is set up to be read by ACCELEROMETER_READ_PERIOD
// #define RUNNING_ARRAY_SIZE 256
// uint16_t g_runningAvgArray[RUNNING_ARRAY_SIZE];
// uint16_t g_runningIndex = 0;

void setup() {
//#ifndef ESP8266
//  while (!Serial) yield();  // will pause Zero, Leonardo, etc until serial console opens
//#endif

  //Serial.begin(300);
  //Serial.println("Starting up...");

  setupAccelerometer();

  // initialize our running array
  // for (int i = 0; i < RUNNING_ARRAY_SIZE; i++) {
  //   g_runningAvgArray[i] = 0;
  // }

  // Set up LEDs
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  // Set up MQTT
  //setupMQTT();
  udpSetup();
}


//Global timers for keeping track of loop things
//unsigned long g_timer_0 = 0;
// Cycle for LED blinking
//#define CYCLE_TIME 100


void loop() {

  // Read in accelerometer data
  //uint32_t avg = 0;
  haveNewAccelData();
  checkPacketContent();
  // if (haveNewAccelData()) {
  //   //if (shouldWeReadAccelerometer(ACCELEROMETER_READ_PERIOD)){
  //   //struct AccelerometerData aData = readAccelerometer();
  //   struct AccelerometerData aData = currentAccelData;

  //   //    //Serial.print("X:  "); //Serial.print(aData.x);
  //   //    //Serial.print("  \tY:  "); //Serial.print(aData.y);
  //   //    //Serial.print("  \tZ:  "); //Serial.println(aData.z);

  //   // Interpret accelerometer data
  //   // adding data to running array
  //   g_runningAvgArray[g_runningIndex] = aData.z;
  //   g_runningIndex++;
  //   if (g_runningIndex >= RUNNING_ARRAY_SIZE) {
  //     g_runningIndex = 0;
  //   }
  //   uint32_t sum = 0;
  //   for (int i = 0; i < RUNNING_ARRAY_SIZE; i++) {
  //     sum += g_runningAvgArray[i];
  //   }
  //   avg = sum / RUNNING_ARRAY_SIZE;
  //   //    //Serial.print("Average: ");
  //   //    //Serial.println(avg);
  //   char buffer[5];
  //   itoa(avg, buffer, 10);
  //   //mqttClient.publish("IROS/accelerometer", 0, true, buffer);
  // }
  // Write LEDs
  ///uint8_t hue = avg / 4; // divided by 4, since we're currently using an analog read that goes 0-1024
  //  delay(1000);
  //uint8_t hue = gHue;
  //uint8_t pot = gpot;
  //  //Serial.print ("gHue: ");
  //  //Serial.println(gHue);
  //  //Serial.print ("Hue: ");
  //  //Serial.println(hue);

  // Using hue instead of color, which we get from our mqtt topic! now we gotta set all the leds
  // and do what we wanna do with the stomp bit

  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    // Turn our current led on to white, then show the leds
    //      leds[whiteLed] = CRGB::Violet;

    // Show the leds (only one of which is set to white, from above)


    // Wait a little bit
    //delay(100);
    sat = 255;
    bright = 150;
    // if (pot < 50) {
    if (pot_state == 0) {
      // Turn our current led back to black for the next loop around
      color = 96;
      //        //Serial.println(color);
      bright2 = 100;
      leds[whiteLed] = CHSV(color, sat, bright);
      wait = 900;
    // } else if (pot < 100) {
    } else if (pot_state == 1) {
      color = 64;
      bright2 = 75;
      leds[whiteLed] = CHSV(color, sat, bright);
      wait = 700;
    // } else if (pot < 150) {
    } else if (pot_state == 2) {
      color = 40;
      bright2 = 50;
      leds[whiteLed] = CHSV(color, sat, bright);
      wait = 500;
    // } else if (pot < 200) {
    } else if (pot_state == 3) {
      color = 20;
      bright2 = 25;
      leds[whiteLed] = CHSV(color, sat, bright);
      wait = 300;
    } else {
      color = 0;
      bright2 = 0;
      leds[whiteLed] = CHSV(color, sat, bright);
      wait = 100;
    }
    //        //Serial.print("LED loop: ");
    //      //Serial.println(color);
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

  //      //Serial.println(color);
  //      //Serial.println(wait);
  FastLED.show();
  delay(wait);

  // If a person is here, blink
  // if (hue < 99) {
  if (distance == 1) {

    //          if (abs(distance-prev_dist) < 55){
    //            //Serial.print("Distance: ");
    //            //Serial.println(hue);
    for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      //              //Serial.print("Flash loop: ");
      //            //Serial.println(whiteLed);
      leds[whiteLed] = CHSV(color, sat, bright2);
    }
    //        }
  }
  /// AND BLINK AT FIXED RATE FOR STOMP
  struct AccelerometerData aData = currentAccelData;
  if (aData.z > 10) {
    //        //Serial.print("Here");
    for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      bright = 255;
      leds[whiteLed] = CHSV(color, sat, bright);
    }
    wait = 100;
  }
  FastLED.show();
  delay(wait);
  //      //Serial.print("Brightness: ");
  //      //Serial.println(bright);
  //prev_dist = distance;
}

unsigned long g_accel_timer = 0;
bool shouldWeReadAccelerometer(long readPeriod) {
  if (g_accel_timer < millis()) {
    g_accel_timer = millis() + readPeriod;
    return true;
  } else {
    return false;
  }
}
