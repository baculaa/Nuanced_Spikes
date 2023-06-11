#ifndef ACCELEROMETER_CODE_H
#define ACCELEROMETER_CODE_H

#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include "pindefs.h"


// Set to zero if using the I2C accelerometer
bool ANALOG_ACCELEROMETER = 0;

struct AccelerometerData {
  int x;
  int y;
  int z;
};

void setupAccelerometer();
AccelerometerData readAccelerometer();

// I2C accelerometer sensor
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setupAccelerometer(){
  if (ANALOG_ACCELEROMETER != 1){
    if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
      Serial.println("Couldnt start I2C accelerometer, defaulting to analog read");
      ANALOG_ACCELEROMETER = 1;
    }
    else{
      Serial.println("LIS3DH found!");
      
      lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
      
      Serial.print("Range = "); Serial.print(2 << lis.getRange());  
      Serial.println("G");
    }
  }
}

AccelerometerData readAccelerometer(){
  struct AccelerometerData data;
  if (ANALOG_ACCELEROMETER != 1){
    lis.read();
    data.x = lis.x;
    data.y = lis.y;
    data.z = lis.z;
  }
  else{
    data.z = analogRead(ANALOG_ACCELEROMETER_PIN);
  }
  
  return data;
}


#endif