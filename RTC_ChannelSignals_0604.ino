//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code
//The examples from https://www.arduinolibraries.info/libraries/ds3231 were very helpful with the implementation of the RTC
//The DS3132 library was used for the RTC
#include <DS3231.h>  //This is the library for the RTC 
#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#define interruptPin 13 //Pin we are going to use to wake up the Arduino
DS3231 rtc;
IRsend irsend;
int RECV_PIN = 1;
IRrecv irrecv(RECV_PIN);
decode_results results;
int jstickVER_pin = A7;
int jstickHOR_pin = A1;
//the horizontal and vertical pins are swapped because of the way the joystick will be oriented
int jstickclick_pin = 10; 
boolean debounceButton(int buttonPin);


//measured using IRRecvDumpv2 and Arduino serial
//This is where you will need to enter your oln codes that you take from the IR receiver 
  unsigned int  NEC1[35] = {8850, 4400, 500, 4400, 500, 2200, 500, 2200, 500, 2150, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 4400, 450, 4400, 500, 4400, 500, 4400, 500}; // UNKNOWN 92DF9279
unsigned int  NEC2[35] = {8850, 4400, 500, 2200, 450, 4450, 500, 2150, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2150, 500, 2200, 500, 4400, 500, 4400, 500, 4400, 500}; // UNKNOWN 87CDD0EF
unsigned int  NEC3[35] = {8850, 4400, 500, 4400, 500, 4400, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2150, 500, 4400, 500, 2200, 500, 4400, 500, 4400, 500}; // UNKNOWN 37788763
unsigned int  NEC4[35] = {8800, 4400, 500, 2200, 500, 2200, 500, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 4400, 500, 4400, 500}; // UNKNOWN A519853B
unsigned int  NEC5[35] = {8850, 4400, 500, 4400, 500, 2200, 450, 4400, 500, 2200, 500, 2200, 500, 2200, 450, 2250, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2150, 500, 4400, 500, 4400, 500, 2200, 500, 4400, 500}; // UNKNOWN 5CDD8FBD
unsigned int  NEC6[35] = {8850, 4400, 500, 2150, 500, 4400, 500, 4400, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 4400, 500, 2200, 450, 4450, 450}; // UNKNOWN 42DD49
unsigned int  NEC7[35] = {8850, 4400, 500, 4400, 500, 4400, 500, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 4400, 500, 2150, 500, 2200, 500, 4400, 500}; // UNKNOWN 31BB009F
unsigned int  NEC8[35] = {8850, 4400, 500, 2200, 500, 2200, 450, 2200, 500, 4400, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 4400, 500}; // UNKNOWN 153F90A7
unsigned int  NEC9[35] = {8850, 4400, 500, 4400, 500, 2200, 450, 2200, 500, 4400, 500, 2200, 500, 2200, 500, 2200, 500, 2150, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 4400, 500, 4400, 500, 4400, 500, 2200, 500}; // UNKNOWN 6872B60C
unsigned int  NEC0[35] = {8850, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2150, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500}; // UNKNOWN 6E56F924
unsigned int  TVON[25] = {2400, 600, 1150, 600, 600, 600, 1200, 550, 600, 600, 1200, 550, 650, 550, 600, 550, 1250, 550, 600, 550, 650, 600, 600, 550, 600}; // SONY A90
unsigned int  STBON[35] = {8850, 4400, 500, 2200, 500, 4400, 500, 2200, 500, 4400, 500, 2150, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 4400, 500, 4400, 450, 2200, 500}; // UNKNOWN 736ED82E
unsigned int  VOLUP[25] = {2400, 550, 600, 600, 1200, 550, 600, 600, 600, 600, 1200, 550, 600, 600, 600, 600, 1150, 600, 600, 600, 600, 550, 600, 600, 600}; // SONY 490
unsigned int  VOLDOWN[25] = {2400, 600, 1200, 550, 1200, 600, 600, 550, 600, 600, 1200, 550, 600, 600, 600, 600, 1200, 550, 600, 600, 600, 600, 600, 550, 600}; // SONY C90
unsigned int  CHUP[35] = {8850, 4400, 500, 4400, 500, 4400, 500, 2200, 450, 4400, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 4400, 500, 2150, 500, 4400, 500, 2200, 500}; // UNKNOWN 8A67822
unsigned int  CHDOWN[35] = {8850, 4400, 500, 2200, 500, 2200, 500, 4400, 500, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 4450, 450, 2200, 500}; // UNKNOWN FBACE420
unsigned int  ONDEMAND[35] = {8850, 4450, 450, 2200, 500, 4400, 500, 2200, 500, 4400, 500, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 4400, 500, 2200, 500, 4400, 500, 2200, 500}; // UNKNOWN 87AE0368
unsigned int  LEFT[35] = {8850, 4400, 500, 2200, 500, 4400, 500, 4400, 450, 2200, 500, 4400, 500, 4400, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 4400, 500, 4400, 500, 4400, 450, 2200, 500}; // UNKNOWN 9578646A
unsigned int  RIGHT[35] = {8850, 4400, 500, 4400, 500, 4400, 450, 4450, 450, 2200, 500, 4400, 500, 4400, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 4400, 500, 4400, 500, 2200, 500}; // UNKNOWN DC18602C
unsigned int  UP[35] = {8850, 4400, 500, 2200, 450, 2200, 500, 4400, 500, 2200, 500, 4400, 500, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 4400, 500, 2200, 500, 2200, 500, 4400, 500}; // UNKNOWN 2C2E80FF
unsigned int  DOWN[35] = {8850, 4400, 500, 4400, 500, 2150, 500, 4400, 500, 2200, 500, 4400, 500, 4400, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 2200, 500, 2200, 500, 4400, 500}; // UNKNOWN 5A1A483D
unsigned int  OK[35] = {8850, 4400, 500, 4400, 500, 2200, 450, 2200, 500, 2200, 500, 4400, 500, 2200, 500, 2200, 500, 2150, 500, 2200, 500, 2200, 500, 2200, 500, 2200, 450, 2200, 500, 4400, 500, 4400, 500, 4400, 500}; // UNKNOWN CB3CC07F

int xVal;
int yVal;
int Click;


const int IRledPin =  3;    // LED connected to digital pin 13
const int LEDalert = 2;      //LED connected to digital pin 2 for the RTC alert
//long int channel1 [] = {824, 9180, 4580, 500, 4580, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 4560, 520, 4560, 520, 4580, 500, 4580, 500, 31480, 9200, 2260, 520, 24644, 9180, 2280, 520};
//long int channel2 [] = {35164, 9200, 4580, 500, 2280, 520, 4560, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 4580, 500, 4580, 500, 4580, 520, 34580, 9180, 2280, 520};
//long int channel3 [] = {6284, 9180, 4580, 500, 4580, 520, 4560, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 4580, 500, 2280, 520, 4560, 520, 4580, 500, 31480, 9200, 2260, 520, 24644, 9200, 2260, 520};

//favchannels[] = {1,2,3};
int i = 0;           //initializes the counter i to 0

const int TVonoffpin = 4;     // TV
const int voluppin = 7;    // Volume Up 
const int voldownpin = 8;     // Volume Down
const int STBonoffpin = 5;    // STB
const int channelpin = 6;     // Channels
const int ondemandpin = 9;    // OnDemand

// The setup() method runs once, when the sketch starts

void setup()   {
   Serial.begin(9600);
   Serial.print("in setup loop 1");

  // initialize the IR digital pin as an output:

//   if (!rtc.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    // following line sets the RTC to the date & time this sketch was compiled
//    rtc.adjust(DateTime(__DATE__, __TIME__));
//  }
//}
  
  pinMode(IRledPin, OUTPUT);

  Wire.begin();

  //this initializes the RTC and a pin as the output for an LED that will alert the user

  pinMode(LEDalert, OUTPUT);
  //The values below set the state for the RTC they should be enetered when the RTC is connected
//  rtc.setClockMode(false);
  pinMode(jstickVER_pin, INPUT);
  pinMode(jstickHOR_pin,INPUT);
  pinMode(jstickclick_pin,INPUT);
  pinMode(channelpin, INPUT);
  pinMode(voluppin, INPUT);
  pinMode(voldownpin, INPUT);
  pinMode(TVonoffpin, INPUT);
  pinMode(ondemandpin, INPUT);
  pinMode(STBonoffpin, INPUT);
 pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor

}

void loop()
{
// Serial.print("in void loop");
  xVal = analogRead(jstickHOR_pin);
  yVal = analogRead(jstickVER_pin);
  Click = digitalRead(jstickclick_pin);
  

if(xVal<18) {
  irsend.sendRaw(RIGHT,sizeof(RIGHT),38);
}


if(xVal>300) {
  irsend.sendRaw(LEFT,sizeof(LEFT),38);
}

if(yVal<300) {
  irsend.sendRaw(UP,sizeof(UP),38);
}
if(yVal>700) {
  irsend.sendRaw(DOWN,sizeof(DOWN),38);
}  
if(Click==0) {
 irsend.sendRaw(OK,sizeof(OK),38);
}
  // Checks if the buttons have been pressed
  
// Serial.println("B4 tim3ch3ck");
//// timeCheck();
// Serial.println("After tim3ch3ck");
//
//Serial.println("I am about to debounce");
  //Volume up
 if(debounceButton(voluppin)==true){
  Serial.print("VOLUP");
   irsend.sendRaw(VOLUP,sizeof(VOLUP),38);
   delay(100);
 }

  //Volume down
 if(debounceButton(voldownpin)==true){
    Serial.print("VOLDOWN");
  irsend.sendRaw(VOLDOWN,sizeof(VOLDOWN),38);
 }

  //TV On/off
if(debounceButton(TVonoffpin)==true){
   Serial.print("TVONOFF");
  irsend.sendRaw(TVON,sizeof(TVON),38);
 }
 
 //STB On/off
 
 if(debounceButton(STBonoffpin)==true){
   Serial.print("STBONOFF");
  irsend.sendRaw(STBON,sizeof(STBON),38);
 }

 if(debounceButton(ondemandpin)==true){
   Serial.print("ONDEMAND");
  irsend.sendRaw(ONDEMAND,sizeof(ONDEMAND),38);
 }

  // If the button is pressed, enter the channel loop
  for(int i=0;i<2;i++){
  if (debounceButton(channelpin) == true) {
    digitalWrite(LEDalert, LOW);
    Serial.println("In button state low");
    if (i == 0) {
      Serial.println("In if loop");
      //    irsend.sendSony(0xa90, 12);
      delay(100); 
      irsend.sendRaw(NEC4, sizeof(NEC4), 38);
      break; 
    }
    
    
    // Next Channel
    else {
      irsend.sendRaw(NEC5, sizeof(NEC5), 38);
           
      delay(100);
      i=0;
      break;
    }
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
  int STBonofflbs=LOW;  
  int channellbs=LOW;   
  int ondemandlbs=LOW;
  
  boolean debounceButton(int buttonPin){
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;    // the debounce time; increase if the output flickers

int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin
  
    switch(buttonPin) {
      
      case TVonoffpin:
//      buttonState = TVonoffbs;             
      lastButtonState = TVonofflbs;   
      break;
      
      case voluppin:
//      buttonState = volupbs;             
      lastButtonState = voluplbs;   
      break;

      case voldownpin:
//      buttonState = voldownbs;             
      lastButtonState = voldownlbs;
      break;
      
      case STBonoffpin:
//      buttonState = STBonoffbs;             
      lastButtonState = STBonofflbs;
      break;

      case channelpin:
//      buttonState = channelpinbs;             
      lastButtonState = channellbs;
      break;
      
      case ondemandpin:   
//      buttonState = ondemandbs;             
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
//      buttonState = TVonoffbs;             
      TVonofflbs = lastButtonState ;   
      break;
      
      case voluppin:
//      buttonState = volupbs;             
      voluplbs = lastButtonState;   
      break;

      case voldownpin:
//      buttonState = voldownbs;             
      voldownlbs = lastButtonState;
      break;
      
      case STBonoffpin:
//      buttonState = STBonoffbs;             
      STBonofflbs = lastButtonState;
      break;

      case channelpin:
//      buttonState = channelpinbs;             
      channellbs = lastButtonState;
      break;
      
      case ondemandpin:   
//      buttonState = ondemandbs;             
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


