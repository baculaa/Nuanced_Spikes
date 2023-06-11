#include <FastLED.h>

#define NUM_LEDS 32
#define DATA_PIN 3
const int trigPin = 9;
const int echoPin = 10;


float duration, distance;
int wait;
int flag;

CRGB leds[NUM_LEDS];


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(9600);

}

void loop() {
  // Move a single white led 

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
  
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
//      leds[whiteLed] = CRGB::Violet;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      //delay(100);

        if (analogRead(A1) < 200) {
        // Turn our current led back to black for the next loop around
        leds[whiteLed] = CRGB::Green;
        wait = 900;
        }
        else if (analogRead(A1) < 400) {
        leds[whiteLed] = CRGB::Yellow;
        wait = 700;
        }
        else if (analogRead(A1) < 600) {
        leds[whiteLed] = CRGB::Orange;
        wait = 500;
        }
        else if (analogRead(A1) < 800) {
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
