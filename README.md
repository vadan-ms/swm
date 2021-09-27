# SWM
Smart Water Meter is a device which monitors the water comsumption by an entity for any required time, subjecting to the user requirements, the obtained data can be futher saved for documentation in the cloud which can be used for analytics.

## How to run
1. Download Arduino IDE https://www.arduino.cc/en/software.
2. Download and Install all the dependencies given below.
3. Create a new .ino file and paste the code or upload the downloaded SWM.ino file directly.
4. Create a new ThingsSpeak https://thingspeak.com/ account and channel for tracking the data note down the channel ID and Key.
5. Change the channel ID and Key in the code to your requirements, given in line 30 and 31 of SWM.ino.
6. Setup wifi access to the ESP8266 by changing the wifi SSID and Password to your desired network, given in line 56 of SWM.ino file.
4. Check for any compilation errors.
5. Upload the code onto your Arduino.

## Checking on output
1. After executing the code you can observe the output in Serial Window of Arduino IDE or your connected peripheral display.
2. Wait for the ESP module to connect to the wifi.
3. The output screen should be updated as defined by user in line 104 of SWM.ino file.

## Dependencies
1. EEPROM: library for Arduino IDE to make code compatible with ESP8266 microcontroller.
2. ESP8266WiFi: library for Arduino IDE for settingup the wifi module of ESP8266.
3. ThingSpeak: library for Arduino IDE for uploading the data to cloud.
4. Adafruit_GFX: library for Arduino IDE to enable graphical features of the oled display (ST7789).
5. Adafruit_ST7789: library for Arduino IDE for interfacing the oled display (ST7789).
