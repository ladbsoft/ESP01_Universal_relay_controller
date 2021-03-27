# ESP01_Universal_relay_controller
### A project to control a standard ESP-01 relay board over MQTT

![Board](https://github.com/ladbsoft/ESP01_Universal_relay_controller/blob/main/photo.jpg "Board")

## About this
A simple controller for the well-known relay controller board that you can find in most eBay or Aliexpress electronic shops. It uses MQTT to receive commands and broadcast status. It has a watchdog that guarantees the board will be ready for commands, and it remembers the last status, in case of power losses.

## Installation
You'll need:

### Hardware
- ESP-01 + Relay controller board (You can easily find them on [Aliexpress](https://www.aliexpress.com/wholesale?SearchText=ESP-01+relay)

### Software
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- The [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) library
- The [PubSubClient](https://github.com/knolleary/pubsubclient) library
- The [ESP8266 WiFiManager](https://github.com/tzapu/WiFiManager) library
- An MQTT server to communicate to (in my case I used the embedded MQTT server from [OpenHAB](https://www.openhab.org/)

### Configuration of the Arduino Sketch
You'll need to configure in Configuration.h the IP, user, and password of your MQTT server, the client ID, and the MQTT topics to use and an SSID and password to create a new WiFi connection with a captive portal, that lets you connect to your WiFi router. This is not your actual WiFi router SSID and password, but a new one. Once you power on the microcontroller for the first time, connect to the SSID you configured here with your password and navigate to the captive portal to then configure your real WiFi connection.
