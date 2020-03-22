//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code
//The examples from https://www.arduinolibraries.info/libraries/ds3231 were very helpful with the implementation of the RTC
//The DS3231 library was used for the RTC
#include <DS3231.h>  //This is the library for the RTC 
#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <SD.h>
#include <SPI.h>
DS3231 rtc;
IRsend irsend;
int RECV_PIN = 1;
IRrecv irrecv(RECV_PIN);
decode_results results;

/*long NEC_1 = 0x21;
  long NEC_2 = 0x22;
  long NEC_3 = 0x23;
  long NEC_4 = 0x25;
  long NEC_5 = 0x26;
  long NEC_6 = 0x27;
  long NEC_7 = 0x29;
  long NEC_8 = 0x2A;
  long NEC_9 = 0x2B;
  long NEC_0 = 0x28;
*/
//measured using IRRecvDumpv2 and Arduino serial
//This is where you will need to enter your oln codes that you take from the IR receiver 

/*
  PURPOSE: These are the channel signals which we measured from our client's television remote. You will need an infrared receiver
  such as the one found at https://www.sparkfun.com/products/10266 in order to measure these signals.
*/
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

int CS_PIN = 10;       // SD Pin
int counter = 1;       // Counter
int c = 1;             // Counter 2
long channel;          // Sent Channels
File file;

int IRledPin =  3;    // LED connected to digital pin 13
int buttonPin = 5;      //Button connected to pin 3
int LEDalert = 12;      //LED connected to digital pin 12 for the RTC alert
//long int channel1 [] = {824, 9180, 4580, 500, 4580, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 4560, 520, 4560, 520, 4580, 500, 4580, 500, 31480, 9200, 2260, 520, 24644, 9180, 2280, 520};
//long int channel2 [] = {35164, 9200, 4580, 500, 2280, 520, 4560, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 4580, 500, 4580, 500, 4580, 520, 34580, 9180, 2280, 520};
//long int channel3 [] = {6284, 9180, 4580, 500, 4580, 520, 4560, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 4580, 500, 2280, 520, 4560, 520, 4580, 500, 31480, 9200, 2260, 520, 24644, 9200, 2260, 520};

//favchannels[] = {1,2,3};
int i = 0;           //initializes the counter i to 0
int buttonState;   //initializes the button reader variable

// The setup() method runs once, when the sketch starts

void setup()   {
  // initialize the IR digital pin as an output:
  pinMode(IRledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
  Wire.begin();


  //this initializes the RTC and a pin as the output for an LED that will alert the user

  pinMode(LEDalert, OUTPUT);
  Wire.begin();

  //The values below set the state for the RTC they should be enetered when the RTC is connected
  rtc.setClockMode(false);

  
}

void loop()
{
  // Checks if the buttons have been pressed
  //timeCheck();
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);

  // If the button is pressed, enter the channel loop
  if (buttonState == LOW) {
    digitalWrite(LEDalert, LOW);
    Serial.println("In button state low");
    if (i == 0) {
      Serial.println("In if loop");
      //    irsend.sendSony(0xa90, 12);
      digitalWrite(LEDalert, HIGH);
      digitalWrite(LEDalert, LOW);
      delay(100);
      i++;
 
      irsend.sendRaw(NEC2, sizeof(NEC2), 38);
     
    }
    
     delay(100);
      i++;
    }
    // Next Channel
    else if (i == 1) {
      irsend.sendRaw(NEC5, sizeof(NEC5), 38);
           
      delay(100);
      i++;
    }

    else {

      irsend.sendRaw(NEC7, sizeof(NEC7), 38);
      delay(100);
      i = 0;
    }
  
}
// Checks RTC Time
  void timeCheck(){
  bool h12 =false;
  bool PM =false;
  int h=rtc.getHour(h12, PM);   //these are both false because the rtc is in military mode
  int m=rtc.getMinute();
  int s=rtc.getSecond();
                             //the first parameter is checking if it is the 12 hour and the second is checking if it is PM
  if(buttonPin==HIGH){                //when the button is pressed the LED will turn off if not the LED will remain on from 5 minutes before the show to the time of the show
    digitalWrite(LEDalert, LOW);
    digitalWrite(IRledPin,LOW);
    Serial.println("in button off");
  }

  else if(h==7 && m==54 && s==3){             //This is an alert to turn on the television at 8 am to channel 5
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




