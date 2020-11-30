# Joystick-Controlled-Quade

## Project Goal

Creating a quadraped robot that uses an ultra sonic sensor and joystick to allow wireless control. The information from the ultra sonic sensor will allow the controller to guide the robot even if the controller cannot see where it is.

## MSP432 UART Connection

Based off [TI UART example code](http://dev.ti.com/tirex/explore/node?node=AHgBmlbzFK.HTw3HuKF7Cw__z-lQYNj__LATEST)
This example was chosen as the base because the baud rate was already set at 9600 which is necessary for communication with the Arduino Uno.

Altered to allow communication over the 3.2 and 3.3 pins on the MSP432 so that it can be connected to and Arduino Uno. It works with a receive interrupt that will then transmit the information from the ultra sonic sensor. 

## Arduino Blynk Code

Based off examples from the Blynk library, [Joystick Example](https://examples.blynk.cc/?board=Arduino%20Uno&shield=ESP8266%20WiFi%20Shield&example=Widgets%2FJoystickTwoAxis) and [Virtual Display](https://examples.blynk.cc/?board=Arduino%20Uno&shield=ESP8266%20WiFi%20Shield&example=GettingStarted%2FVirtualPinWrite).
Our code combines these examples and alters the display so that it is comstantly updating whenever new data is available.

This was an easy way to have a wireless connection with the MSP432 that just needed the addition of the UART communication to work with the TX and RX pins. The code creates an array for the joystick inputs and writes the sensor values to a digital display.
