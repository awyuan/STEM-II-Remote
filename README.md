# STEM-II-Remote

This repository is for the Assistive Remote developed in 2018 as a Mass Academy STEM II Project.
The remote was developed by Krish Nathan, Victoria Bowen, Andrew Yuan, and Joyce Fang.

If you have any questions about the code here, you can reach out to Krish at krishnathan2019@gmail.com.

## Working Code
The working code which we uploaded to our client's remote is *Working_RTC_ChannelSignals.ino*. This sketch incorporates the infrared LED, the real time clock (RTC), and the joystick. You will need to make a .txt file named *channels.txt* with each channel

However, if you want a simpler sketch which just contains essential remote code, check out *Remote_Example_revised.ino*. The sketch only contains the code for sending different channel signals in a cycle. The sketch is easier to understand, and it works if you do not need an RTC or joystick.

## Arduino Board
We used the [Arduino Pro Mini board](https://www.sparkfun.com/products/11113)
for our remote. If you wish to use a board which supports Wifi and Bluetooth, I would recommend the [ESP32 Board](https://amzn.to/2UubU7p).

## Buttons
We used these [large, colorful buttons](https://amzn.to/33Aa4Gi) that are easy to see and press. They are also pull up buttons, which
means that they read as HIGH when released and LOW when pressed. This is useful because it eliminates false positive presses caused by background noise.

## Infrared Library

We utilized an [infrared library](https://github.com/z3t0/Arduino-IRremote) for this project

### Measuring remote signals: 

In order to duplicate the actions produced by your client's original remote, you will need to measure the signals produced by each button. These are the steps

1. Connect an infrared receiver such as the one found [here](https://www.sparkfun.com/products/10266) to your Arduino.
2. Install the infrared library to your computer using the instructions from the [infrared library](https://github.com/z3t0/Arduino-IRremote)
3. Open the [infrared receiving demo](https://github.com/z3t0/Arduino-IRremote/blob/master/examples/IRrecvDumpV2/IRrecvDumpV2.ino)
in your arduino IDE and change the *recvPin* field to match the pin your receiver is connected to.
4. Upload the sketch to your Arduino and open the Serial Monitor.
5. Point your client's television remote directly at your infrared receiver
6. Press the television button whose infrared signal you wish to measure

### Sending remote signals

Check out [this](https://github.com/z3t0/Arduino-IRremote/blob/master/examples/IRsendRawDemo/IRsendRawDemo.ino) demo if you want to see how to send **raw signals** from the infrared LED. Raw signals are an array of integers which represents alternating delays and 38Khz pulses. Each integer is the length of the delay or pulse in microseconds. This is how we ended up sending signals from our remote

Check out [this](https://github.com/z3t0/Arduino-IRremote/blob/master/examples/IRsendDemo/IRsendDemo.ino) demo if you want to see how to send **hexadecmial signals**. You should be able to measure these using the receiving demo above. It could be less complicated to send these types of signals.

## RTC library
We used the [DS3231 RTC](https://amzn.to/2Uaj8ik) in order to keep track of the time, so that we could automatically switch the channel on our client's television each hour. For a smaller, more convenient RTC consider [this one](https://www.adafruit.com/product/3013).
We modified the starting code of a button from [this article](http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/).
Tutorials and examples found [here](https://www.arduino.cc/en/Tutorial/Button) were used to help with the code.
The [examples](https://www.arduinolibraries.info/libraries/ds3231) were very helpful with the implementation of the RTC.
This [DS3231 library](https://github.com/NorthernWidget/DS3231) was used for the RTC.

## Joystick
We used this [joystick](https://amzn.to/2J6GcYY) in order to allow our client to scroll through an on-demand menu using up, down, left, and right controls.

## SD Card Reader
We used this [SD card reader](https://amzn.to/2xiV83b) to store .txt files containing our channel signals. If you wish to develop a mobile application which interfaces with the television remote, the extra data storage provided by the SD Card would also be useful.

