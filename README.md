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
|  `start`  | POST |time [ms]| Starts the countdown timer with the given time.         |
|  `stop`   | POST |time [ms]| Stops the countdown timer and displays the finish time. |
|  `resume` | GET  |    -    | Restarts the countdown.                                 |
| `disable` | GET  |    -    | Deactivates the timer, so all lights are off.           |

## 🔧 Setup
Open the `timer.ino` in the [Arduino IDE](https://www.arduino.cc/en/Main/Software).   
Change the variables `ssid` and `password` to match your W-LAN credentials.

Connect the the Feather board with the 7-Segment Serial Display. Add a 5V power supply.

| Feather - PIN | Connect | 7 Segment - PIN |
|:-------------:|:-------:|:---------------:|
|    **VCC**    |   ⟷    |     **USB**     |
|    **GND**    |   ⟷    |     **GND**     |
|    **RX**     |   ⟷    |     **16**      |
