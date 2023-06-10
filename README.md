# Nuanced_Spikes

## Components

### Shoes
#### Bill of Materials
Per Shoe

 - 32x 4-pin addressable RGB LEDs, WS2811 chip architecture, RGB color order
 - D1 mini esp382 wifi microcontroller
 - 1Ah lipo
 - lipo charger + conversion to 5V power circuit
 - Some sort of accelerometer

#### Microcontroller code
D1 mini board is the WeMos D1 R2 & mini board from the esp8266 boards manager that we get from the additonal boards manager URL: ```https://arduino.esp8266.com/stable/package_esp8266com_index.json```. Additional details can be found [here](https://makersportal.com/blog/2019/6/12/wemos-d1-mini-esp8266-arduino-wifi-board).

The board we are using can be found in Aruduino under Tools->Board->esp8266->LOLIN(WEMOS) D1 R2 & mini

NOTE! In order to use this logging code, you have to be running the Legacy 1.x IDE, so you can use the ESP data upload tool.
Probably not something that will be ultimately necessary for the actual project, just an annoying step for now for data logging
Currently, just using the Examples -> ESP8266WebServer -> Graph
Additionally, for the logging demo code, you'll need to install a tool following the instructions here: ```https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#uploading-files-to-file-system```, get the 

LED control is impletemented through the [FastLED](https://fastled.io/) package.

### Jacket
#### Bill of Materials

- Raspberry Pi
