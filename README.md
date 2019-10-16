# 💣 ExitGame Bomb - ⏲ Countdown Timer
[![Arduino](https://img.shields.io/badge/Arduino-00979D.svg?style=flat&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![ESP8266WiFi](https://img.shields.io/badge/lib-ESP8266WiFi-blue.svg)](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
[![aREST](https://img.shields.io/badge/lib-aREST-blue.svg)](https://www.arduinolibraries.info/libraries/a-rest)
[![SoftwareSerial](https://img.shields.io/badge/lib-SoftwareSerial-blue.svg)](https://www.arduino.cc/en/Reference/SoftwareSerial)

This countdown timer is part of an [**ExitGame**](https://github.com/TobseF/its-exit-game-bomb) where payers have to deactivate a bomb by coding challenges.  
The timer runs a second based countdown in the format "00:00".  
The countdown can be controlled by a REST interface.  

## 💡 Hardware
The code runs on an [Arduino](https://www.arduino.cc/) compatible [Adafruit Feather HUZZAH with ESP8266](https://www.adafruit.com/product/2821).  
It controls a [SparkFun 7-Segment Serial Display](https://www.sparkfun.com/products/11441).
So wee don't need to manual control each segment. Instead serial data can be sent. 

## 🔌 Endpoints

|    Name   |Method|  Param  |                      Description                        |
|:---------:|:----:|:-------:|---------------------------------------------------------|
|  `start`  | GET |time [ms]| Starts the countdown timer with the given time.         |
|  `stop`   | GET |time [ms]| Stops the countdown timer and displays the finish time. |
|  `resume` | GET  |    -    | Restarts the countdown.                                 |
| `disable` | GET  |    -    | Deactivates the timer, so all lights are off.           |

## 🔧 Setup

1. Install [Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. [Setup the Arduino IDE for the Adafruit Feather HUZZAH](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide)
3. Install the `aREST` lib:  
   _Tools_ > _Manage Libraries.._ Search & install `aREST` (v2.7.5 by Marco Schwartz)
4. Open the `timer.ino` in the [Arduino IDE](https://www.arduino.cc/en/Main/Software).   
5. Change the variables `ssid` and `password` to match your W-LAN credentials.
6. _Verify_, _Compile_ & _Upload_ the sketch to the board.
6. Connect the the _Feather board_ with the _7-Segment Serial Display_. Add a 5V power supply:  
   The timer shows "Connecting" and "Ready" after connecting to a WLAN.
  
  | Feather - PIN | Connect | 7 Segment - PIN |
  |:-------------:|:-------:|:---------------:|
  |    **VCC**    |   ⟷    |     **USB**     |
  |    **GND**    |   ⟷    |     **GND**     |
  |    **RX**     |   ⟷    |     **16**      |
  
  
  
8. Test the timer by sending a `GET` to it's IP with `start?time=120000`.
