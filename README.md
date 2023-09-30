# Nuanced_Spikes

## Components

### Shoes
#### Bill of Materials
Per Shoe

 - 32x 4-pin addressable RGB LEDs, WS2811 chip architecture, RGB color order - pin diagram below
 - D1 mini esp382 wifi microcontroller
 - 1Ah lipo
 - switch
 - lipo charger + conversion to 5V power circuit (We used PowerBoost 1000C)
 - [Some sort of accelerometer (the LIS3DH)](https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout)


#### Wiring connection list
This table shows what pins are connected to each other. Pin labels on the same row are all connected to each other/are the same node.

| D1 Mini | LIS3DH | Ultrasonic | LED1 | PowerBoost100C | Battery | Switch |
| --------|--------|------------|------|----------------|---------|--------|
|    5V   |  Vin   |     VCC    | Vin  |      5V        | ------- | ------ |
|    G    |  GND   |     Gnd    | GND  |      GND       | ------- | Pin 1  |
|    D1   |  SCL   | ---------- | ---- | -------------- | ------- | ------ |
|    D2   |  SDA   | ---------- | ---- | -------------- | ------- | ------ |
|    D5   | ------ |    TRIG    | ---- | -------------- | ------- | ------ |
|    D6   | ------ |    ECHO    | ---- | -------------- | ------- | ------ |
|    D7   | ------ | ---------- | Din  | -------------- | ------- | ------ |
| ------- | ------ | ---------- | ---- |      Bat       |  Vout   | ------ |
| ------- | ------ | ---------- | ---- | -------------- |   GND   | Common |


##### LED Pinout diagram
This pinout diagram is for our WS2811-style RGB LEDs

```
   _________
 /           \
|             |
|             |
______________
 |   |   |   |
 |   |   |   |
 |   |   |   |
Din  |   |  Dout
    GND  |
        VCC
```


#### Microcontroller Code Packages and Setup
We used the Arduino IDE for programming the D1 Minis.

D1 mini board is the WeMos D1 R2 & mini board from the esp8266 boards manager that we get from the additonal boards manager URL: ```https://arduino.esp8266.com/stable/package_esp8266com_index.json```. Additional details can be found [here](https://makersportal.com/blog/2019/6/12/wemos-d1-mini-esp8266-arduino-wifi-board).

The board we are using can be found in Aruduino under Tools->Board->esp8266->LOLIN(WEMOS) D1 R2 & mini

NOTE! In order to use this logging code, you have to be running the Legacy 1.x IDE, so you can use the ESP data upload tool.
Probably not something that will be ultimately necessary for the actual project, just an annoying step for now for data logging
Currently, just using the Examples -> ESP8266WebServer -> Graph
Additionally, for the logging demo code, you'll need to install a tool [following the instructions here] (https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#uploading-files-to-file-system), get the LittleFS code

[Here is the guide to the LIS3DH library](https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout/arduino), TL;DR, in the Arduino Library Manager, install the **Adafruit LIS3DH** and **Adafruit Unified Sensor** libraries

LED control is impletemented through the [FastLED](https://fastled.io/) package. FastLED by default uses interrupts for timing with the WS2811 LEDs, which can interfere with wifi communication on the esp board (aka, our D1 mini). Information on FastLED interrupt use and problems with other board functionality, as well as potential solutions, are discussed [here](https://github.com/FastLED/FastLED/wiki/Interrupt-problems). Tl;dr: maybe don't do interrupts? Or use shorter LED strings? Or a board with more processing capability?

We're also using MQTT for passing data between the pi and D1 minis.

If problems uploading to D1 Mini, reinstall the CH340 driver from [here](https://sparks.gogo.co.nz/ch340.html)


### Jacket
#### Bill of Materials

- Raspberry Pi
- Webcam x4
- D1 mini esp382 wifi microcontroller
- USB battery with on/off switch
- Potentiometer
- [Flora NeoPixels](https://www.adafruit.com/product/1260) (aka sewable WS2812 RGB LEDs) x7

#### Raspberry Pi setup

- Using the [Raspberry Pi Imager](https://www.raspberrypi.com/software/), select Raspberry Pi OS (other) -> Raspberry Pi OS (64-bit) and write that to the SD card
- Setup Rasbperry Pi as normal, I set user=raspi, pw=pi, set region to United States and/or click the US keyboard layout button, let it update
- Install opencv by opening a terminal and using `pip3 install opencv-python` (Note: make sure to use **pip3**, regular pip won't find a precomplied version, try to compile from source, then fail after many hours)
-  Install the zenoh python package `
-  Install the zenoh server: 
```
echo "deb [trusted=yes] https://download.eclipse.org/zenoh/debian-repo/ /" | sudo tee -a /etc/apt/sources.list > /dev/null
sudo apt update
sudo apt install zenoh 
```
- [Set up the Pi as a WiFi Access Point (AP)](https://www.raspberrypi.com/documentation/computers/configuration.html#setting-up-a-routed-wireless-access-point) - more details are in the Raspberry Pi Code\WiFiSetup.md, if you need more details

#### D1 Mini Setup
Code environment is the same as what's discussed for the shoes. 


##### Wiring connection list

| D1 Mini | Pot | LED1 | RaspberryPi |
| --------|-----|------|-------------|
|    USB  |-----|------|     USB     |
|    5V   | Vin | Vin  | ----------- |
|    G    | GND | GND  | ----------- |
|    A0   | Out | ---- | ----------- |
|    D7   | --- | Din  | ----------- |