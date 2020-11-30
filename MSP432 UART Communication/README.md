# MSP432 UART Communication

## Preface
This code is based off the [TI UART example code](http://dev.ti.com/tirex/explore/node?node=AHgBmlbzFK.HTw3HuKF7Cw__z-lQYNj__LATEST)

## Information
This code is the base for the UART communication through the 3.2 and 3.3 pins that works with a reciving interrupt.
This is used so that we can transmit the ultra sonic sensor values through the UART to the Arduino.

This allows us to utialize that Blynk libraries that work with Arduinos to transmit the information wirelessly using the ESP8266 esp-01 module.
