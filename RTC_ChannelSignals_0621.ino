//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code
//The examples from https://www.arduinolibraries.info/libraries/ds3231 were very helpful with the implementation of the RTC
//The DS3132 library was used for the RTC
//Sleep Demo from https://playground.arduino.cc/Learning/ArduinoSleepCode was used. Below is the comment given in the code about usage rights
/* Sleep Demo Serial
 * -----------------
 * Example code to demonstrate the sleep functions in an Arduino.
 *
 * use a resistor between RX and pin2. By default RX is pulled up to 5V
 * therefore, we can use a sequence of Serial data forcing RX to 0, what
 * will make pin2 go LOW activating INT0 external interrupt, bringing
 * the MCU back to life
 *
 * there is also a time counter that will put the MCU to sleep after 10 secs
 *
 * NOTE: when coming back from POWER-DOWN mode, it takes a bit
 *       until the system is functional at 100%!! (typically <1sec)
 *
 * Copyright (C) 2006 MacSimski 2006-12-30
 * Copyright (C) 2007 D. Cuartielles 2007-07-08 - Mexico DF
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Code Starts Here
#include <DS3231.h>  //This is the library for the RTC 
#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
DS3231 rtc;
IRsend irsend;

int jstickVER_pin = A7;
int jstickHOR_pin = A6;
//the horizontal and vertical pins are swapped because of the way the joystick will be oriented
int jstickclick_pin = 10; 
boolean debounceButton(int buttonPin);

// Sleep Variables
int wakePin = 2;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter


//measured using IRRecvDumpv2 and Arduino serial
//This is where you will need to enter your oln codes that you take from the IR receiver 

int xVal;
int yVal;
int Click;

const unsigned int  PROGMEM SANYO4[47] = {3350,3300, 850,2450, 900,2450, 850,800, 900,2450, 850,2500, 850,2500, 850,2500, 850,800, 850,800, 850,2500, 850,800, 850,800, 900,800, 850,2450, 900,800, 850,800, 850,800, 850,800, 900,2450, 850,2500, 850,800, 850,2500, 850};  // SANYO 37910D
const unsigned int  PROGMEM SANYO5[47] = {3350,3300, 850,2500, 850,2450, 900,800, 850,2450, 900,2450, 900,800, 850,800, 850,2500, 850,800, 850,2500, 850,800, 850,800, 850,800, 900,2450, 850,800, 900,800, 850,2450, 900,2450, 900,800, 850,2450, 900,800, 850,2400, 950};  // SANYO 365135
const unsigned int  TVON[47] = {3350,3300, 850,2500, 850,2500, 850,800, 850,2500, 850,2500, 800,2500, 850,2500, 850,2500, 850,800, 850,850, 800,850, 850,800, 850,800, 850,2500, 850,800, 850,850, 800,850, 800,850, 850,800, 850,2500, 850,2500, 850,2500, 800};  // SANYO 37C107
const unsigned int  CBUTTON[47] = {3350,3300, 850,2500, 850,2500, 800,850, 850,2500, 800,2500, 850,2500, 850,800, 850,850, 800,850, 850,2500, 800,2550, 800,850, 850,800, 850,2500, 850,800, 850,800, 850,850, 800,2500, 850,2500, 850,2500, 850,800, 850,800, 850};  // SANYO 37191C
const unsigned int PROGMEM  VOLUP[67] = {4500,4400, 650,1550, 650,1600, 600,1600, 600,500, 650,500, 600,500, 600,550, 600,500, 600,1600, 600,1600, 600,1600, 650,500, 600,550, 550,550, 600,500, 600,550, 550,1600, 650,1600, 600,1600, 600,500, 650,500, 600,500, 600,550, 600,500, 600,550, 550,550, 600,500, 600,1600, 600,1600, 650,1550, 650,1600, 600,1600, 600};  // SAMSUNG E0E0E01F
const unsigned int PROGMEM  VOLDOWN[67] = {4500,4400, 650,1600, 600,1600, 600,1600, 600,500, 650,500, 600,500, 650,500, 600,500, 600,1600, 650,1550, 650,1600, 600,500, 600,500, 650,500, 600,500, 600,500, 650,1600, 600,1600, 600,500, 650,1550, 650,500, 600,500, 650,500, 600,500, 600,500, 650,500, 600,1600, 600,500, 650,1550, 650,1600, 600,1600, 600,1600, 600};  // SAMSUNG E0E0D02F
const unsigned int PROGMEM  CHUP[47] = {3400,3250, 900,2450, 850,2500, 850,800, 850,2450, 900,2450, 900,2450, 850,800, 900,2450, 850,2450, 900,2450, 900,750, 900,800, 850,800, 850,2500, 850,800, 850,800, 850,800, 900,2450, 850,800, 850,800, 900,750, 900,2450, 900};  // SANYO 377111
const unsigned int PROGMEM  CHDOWN[47] = {3350,3250, 900,2450, 900,2450, 900,750, 900,2450, 900,2450, 850,800, 900,2400, 900,2450, 900,2450, 900,2450, 900,750, 900,750, 900,750, 900,2450, 900,750, 900,750, 900,2450, 900,750, 900,750, 900,800, 900,750, 900,2400, 900};  // SANYO 36F121
const unsigned int PROGMEM  ONDEMAND[47] = {3400,3250, 900,2450, 900,2450, 900,750, 900,2400, 900,2450, 900,750, 900,800, 850,800, 850,2450, 900,2450, 900,750, 900,800, 850,800, 850,2450, 900,800, 850,800, 850,2500, 850,2400, 950,2450, 900,750, 900,750, 900,2450, 900};  // SANYO 363139
const unsigned int PROGMEM  LEFT[47] = {3350,3250, 900,2450, 900,2450, 850,800, 900,2450, 850,2500, 850,2450, 900,2450, 850,800, 900,750, 900,800, 850,800, 850,800, 850,800, 900,2450, 850,800, 900,750, 900,750, 900,800, 850,2450, 900,2450, 850,2500, 850,2450, 900};  // SANYO 37810F
const unsigned int PROGMEM  RIGHT[47] = {3350,3300, 850,2450, 900,2450, 900,750, 900,2450, 900,2450, 900,750, 900,750, 900,2450, 900,750, 900,750, 900,750, 950,750, 900,750, 900,2450, 900,750, 900,750, 900,2450, 900,2400, 900,800, 900,2400, 900,2450, 900,2450, 900};  // SANYO 364137
const unsigned int PROGMEM  UP[47] = {3400,3250, 900,2450, 850,2500, 850,800, 850,2500, 850,2450, 900,800, 850,2450, 900,750, 900,800, 850,800, 850,800, 850,800, 900,750, 900,2450, 900,750, 900,750, 900,2450, 900,750, 900,2450, 900,2450, 850,2450, 900,2450, 900};  // SANYO 36812F
const unsigned int PROGMEM  DOWN[47] = {3350,3300, 850,2450, 900,2450, 900,750, 900,2450, 900,2450, 850,2500, 850,2450, 900,750, 900,800, 850,800, 850,800, 850,800, 900,750, 900,2450, 900,750, 900,750, 900,800, 850,800, 850,2500, 850,2450, 900,2450, 850,2500, 850};  // SANYO 37810F
const unsigned int PROGMEM  OK[47] = {3350,3300, 850,2450, 900,2450, 900,750, 900,2450, 900,2450, 850,800, 850,800, 900,2450, 850,2500, 850,800, 850,800, 850,800, 900,750, 900,2450, 900,750, 900,750, 900,2450, 900,2450, 850,800, 900,750, 900,2450, 850,2500, 850};  // SANYO 366133


const int IRledPin =  3;    // LED connected to digital pin 13
const int LEDalert = 11;      //LED connected to digital pin 2 for the RTC alert
//long int channel1 [] = {824, 9180, 4580, 500, 4580, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 4560, 520, 4560, 520, 4580, 500, 4580, 500, 31480, 9200, 2260, 520, 24644, 9180, 2280, 520};
//long int channel2 [] = {35164, 9200, 4580, 500, 2280, 520, 4560, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 4580, 500, 4580, 500, 4580, 520, 34580, 9180, 2280, 520};
//long int channel3 [] = {6284, 9180, 4580, 500, 4580, 520, 4560, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 4580, 500, 2280, 520, 4560, 520, 4580, 500, 31480, 9200, 2260, 520, 24644, 9200, 2260, 520};

//favchannels[] = {1,2,3};
int i = 0;           //initializes the counter i to 0

const int TVonoffpin = 2;     // TV
const int voluppin = 5;       // Volume Up 
const int voldownpin = 6;     // Volume Down
const int channelpin = 7;    // STB
const int ondemandpin = 8;     // Channels
const int cbuttonpin = 9;    // OnDemand

// The setup() method runs once, when the sketch starts

void setup()   {
   Serial.begin(9600);
   Serial.println("in setup loop 1");

  // initialize the IR digital pin as an output:

//   if (!rtc.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    // following line sets the RTC to the date & time this sketch was compiled
//    rtc.adjust(DateTime(__DATE__, __TIME__));
//  }
//}
  
  Wire.begin();

  //this initializes the RTC and a pin as the output for an LED that will alert the user
  pinMode(wakePin, INPUT);
  pinMode(IRledPin, OUTPUT);
  pinMode(LEDalert, OUTPUT);
  //The values below set the state for the RTC they should be enetered when the RTC is connected
  rtc.setClockMode(false);
  pinMode(jstickVER_pin, INPUT);
  pinMode(jstickHOR_pin,INPUT);
  pinMode(jstickclick_pin,INPUT_PULLUP);
  pinMode(channelpin, INPUT);
  pinMode(voluppin, INPUT);
  pinMode(voldownpin, INPUT);
  pinMode(TVonoffpin, INPUT);
  pinMode(ondemandpin, INPUT);
  pinMode(cbuttonpin, INPUT);
  //pinMode(wakePin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor

  /* Now it is time to enable an interrupt. In the function call
   * attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
   *
   * B   Name of a function you want to execute while in interrupt A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level trigger
   *             CHANGE     a change in level trigger
   *             RISING     a rising edge of a level trigger
   *             FALLING    a falling edge of a level trigger
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */
 
//  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
                                      // wakeUpNow when pin 2 gets LOW
}

void loop()
{
Serial.println("in void loop");
  
  xVal = analogRead(jstickHOR_pin);
  yVal = analogRead(jstickVER_pin);
  Click = digitalRead(jstickclick_pin);

Serial.println("end read");  

if(xVal<150) {
  irsend.sendRaw(RIGHT,sizeof(RIGHT),38);
  Serial.println("RIGHT");
     delay(500);

}


else if(xVal>800) {
  irsend.sendRaw(LEFT,sizeof(LEFT),40);
  Serial.println("LEFT");
     delay(500);
}

else if(yVal<150) {
  Serial.println("entered up");
  irsend.sendRaw(UP,sizeof(UP),40);
  Serial.println("UP");
     delay(500);

}

else if(yVal>800) {
  irsend.sendRaw(DOWN,sizeof(DOWN),40);
  Serial.println("DOWN");
   delay(100);

}  
if (Click==0) {
 irsend.sendRaw(OK,sizeof(OK),40);
 Serial.println("OK");
   delay(100);

}
  // Checks if the buttons have been pressed
  
// Serial.println("B4 tim3ch3ck");
timeCheck();
// Serial.ln("After tim3ch3ck");
//
//Serial.println("I am about to debounce");
  //Volume up
 if(debounceButton(voluppin)==true){
  Serial.println("VOLUP");
   irsend.sendRaw(VOLUP,sizeof(VOLUP),40);
   delay(100);
 }

//Volume down
else if(debounceButton(voldownpin)==true){
    Serial.println("VOLDOWN");
  irsend.sendRaw(VOLDOWN,sizeof(VOLDOWN),40);
    delay(100);
 }

//TV On/off
else if(debounceButton(TVonoffpin)==true){
   Serial.println("TVONOFF");
  irsend.sendRaw(TVON,sizeof(TVON),40);
  delay(100);
 }
 
//STB On/off
 
else if(debounceButton(cbuttonpin)==true){
   Serial.println("cbutton");
  irsend.sendRaw(CBUTTON,sizeof(CBUTTON),36);
 delay(100);
 }

else if(debounceButton(ondemandpin)==true){
   Serial.println("ONDEMAND");
  irsend.sendRaw(ONDEMAND,sizeof(ONDEMAND),56);
   delay(100);

 }

// If the button is pressed, enter the channel loop
  else if (debounceButton(channelpin) == true) {
    digitalWrite(LEDalert, LOW);
    Serial.println("In button state low");
    if (i == 0) {
      Serial.println("channel 4");
      //    irsend.sendSony(0xa90, 12);
      irsend.sendNEC(0x37910D, 38);
      i++;
    }
    
    
// Next Channel
    else {
      Serial.print("channel 5");
      irsend.sendNEC(0x365135, 38);
      i=0;
    }
  }
}
// Checks RTC Time
  void timeCheck(){
  Serial.println("In tim3ch3ck!!!");
  bool h12 =false;
  bool PM =false;
  int h=rtc.getHour(h12, PM);   //these are both false because the rtc is in military mode
  int m=rtc.getMinute();
  int s=rtc.getSecond();
                             //the first parameter is checking if it is the 12 hour and the second is checking if it is PM
  if(debounceButton(channelpin)==true){                //when the button is pressed the LED will turn off if not the LED will remain on from 5 minutes before the show to the time of the show
    digitalWrite(LEDalert, LOW);
    Serial.println("in button off");
  }

  else if(h==7 && m==55 && s==00){             //This is an alert to turn on the television at 8 am to channel 5
    digitalWrite(LEDalert, HIGH);
    Serial.println("in time alert");
  }
  
  else if(h==13 && m==55 && s==00){           //this is an alert to switch the channel back to channel 4 at 2:00
    digitalWrite(LEDalert, HIGH);
  }
  else if(h==14 && m==55 && s==00 ){          //this is an alert to switch the channel back to channel 5 at 3:00
    digitalWrite(LEDalert, HIGH);
  }
 
  }

   


//this code was adapted from the Arduino DebounceButton example
int TVonoffbs;
int volupbs;     
int voldownbs;    
int STBonoffbs;  
int channelbs;   
int ondemandbs;  

int TVonofflbs=LOW;
int voluplbs=LOW;     
int voldownlbs=LOW;    
int cbuttonlbs=LOW;  
int channellbs=LOW;   
int ondemandlbs=LOW;
  
boolean debounceButton(int buttonPin){
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;    // the debounce time; increase if the output flickers

int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin
  
    switch(buttonPin) {
      
      case TVonoffpin:
//    buttonState = TVonoffbs;             
      lastButtonState = TVonofflbs;   
      break;
      
      case voluppin:
//    buttonState = volupbs;             
      lastButtonState = voluplbs;   
      break;

      case voldownpin:
//    buttonState = voldownbs;             
      lastButtonState = voldownlbs;
      break;
      
      case cbuttonpin:
//    buttonState = STBonoffbs;             
      lastButtonState = cbuttonlbs;
      break;

      case channelpin:
//    buttonState = channelpinbs;             
      lastButtonState = channellbs;
      break;
      
      case ondemandpin:   
//    buttonState = ondemandbs;             
      lastButtonState = ondemandlbs;
      break;
}
  
 
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  // save the reading. Next time through the loop, it'll be the lastButtonState
    lastButtonState = reading;

switch(buttonPin) {
      
      case TVonoffpin:
//    buttonState = TVonoffbs;             
      TVonofflbs = lastButtonState ;   
      break;
      
      case voluppin:
//    buttonState = volupbs;             
      voluplbs = lastButtonState;   
      break;

      case voldownpin:
//    buttonState = voldownbs;             
      voldownlbs = lastButtonState;
      break;
      
      case cbuttonpin:
//    buttonState = STBonoffbs;             
      cbuttonlbs = lastButtonState;
      break;

      case channelpin:
//    buttonState = channelpinbs;             
      channellbs = lastButtonState;
      break;
      
      case ondemandpin:   
//    buttonState = ondemandbs;             
      ondemandlbs = lastButtonState;
      break;
}

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only return true if the new button state is HIGH
      if (buttonState == HIGH) {
          return true;
        }
    }
  }
    return false;  
  }




