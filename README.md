# Nuanced_Spikes

## Components

### Shoes
#### Bill of Materials
Per Shoe

 - 32x 4-pin addressable RGB LEDs, WS2811 chip architecture, RGB color order - pin diagram below
 - D1 mini esp382 wifi microcontroller
 - 1Ah lipo
 - lipo charger + conversion to 5V power circuit
 - [Some sort of accelerometer (the LIS3DH)](https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout)

#### Microcontroller code
[Here is the guide to the LIS3DH library](https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout/arduino), TL;DR, in the Arduino Library Manager, install the **Adafruit LIS3DH** and **Adafruit Unified Sensor** libraries

D1 mini board is the WeMos D1 R2 & mini board from the esp8266 boards manager that we get from the additonal boards manager URL: ```https://arduino.esp8266.com/stable/package_esp8266com_index.json```. Additional details can be found [here](https://makersportal.com/blog/2019/6/12/wemos-d1-mini-esp8266-arduino-wifi-board).

The board we are using can be found in Aruduino under Tools->Board->esp8266->LOLIN(WEMOS) D1 R2 & mini

NOTE! In order to use this logging code, you have to be running the Legacy 1.x IDE, so you can use the ESP data upload tool.
Probably not something that will be ultimately necessary for the actual project, just an annoying step for now for data logging
Currently, just using the Examples -> ESP8266WebServer -> Graph
Additionally, for the logging demo code, you'll need to install a tool [following the instructions here] (https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#uploading-files-to-file-system), get the LittleFS code

LED control is impletemented through the [FastLED](https://fastled.io/) package.

#### LED Pinout diagram
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

### Jacket
#### Bill of Materials

- Raspberry Pi

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
