#ifndef PINDEFS_H
#define PINDEFS_H

/// @file    pindefs.h
/// @brief   Set pins up and explain the overlap nomenclature between Arduino and the D1 mini

// ESP8266's pin names are different than Arduino, they are mapped thus.
//   Several of the pins get used during boot (explained at EOF), and are
//   probably better to be avoided if possible
//   (*asterisk means this pin used on boot)
//   ESP8266   Arduino   |  ESP8266   Arduino
//      A0       A0      |     D6       12
//      D0       16      |     D7       13
//      D1        5      |    *D8*     *15*
//      D2        4      |    *SD2*     *9*
//     *D3*      *0*     |    *SD3*    *10*
//     *D4*      *2*     |    *RX*      *3*
//      D5       14      |    *TX*      *1*

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI


#define DATA_PIN 12 // D6 on the ESP board

// If we're using an analog accelerometer, we'll use an analog pin
#define ANALOG_ACCELEROMETER_PIN A0

// Not needed to define, but our I2C bus for our LIS3whatever
//  I2C accelerometer uses
// 
//  SDA - D2, or A4 on Arduino Uno, digital 3 on Leonardo/Micro
//  SCL - D1, or A5 on Arduino Uno, digital 2 on Leonardo/Micro



// Here's how the ESP8266's troublesome pins are used at boot:
//GPIO16 (D0): pin is high at BOOT
//GPIO0  (D3): boot failure if pulled LOW
//GPIO2  (D4): pin is high on BOOT, boot failure if pulled LOW
//GPIO15 (D8): boot failure if pulled HIGH
//GPIO3  (RX): pin is high at BOOT
//GPIO1  (TX): pin is high at BOOT, boot failure if pulled LOW
//GPIO10 (SD3): pin is high at BOOT
//GPIO9  (SD2): pin is high at BOOT


#endif