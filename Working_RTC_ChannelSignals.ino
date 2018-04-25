//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code 
//The examples from https://www.arduinolibraries.info/libraries/ds3231 were very helpful with the implementation of the RTC 
//The DS3132 library was used for the RTC  
#include <DS3231.h>  //This is the library for the RTC 
#include <Wire.h> 
#include <LiquidCrystal.h>  
#include <IRremote.h>
DS3231 rtc;
IRsend irsend;
int RECV_PIN = 1;
IRrecv irrecv(RECV_PIN);
decode_results results;

long NEC_1 = 0x21;
long NEC_2 = 0x22;
long NEC_3 = 0x23;
long NEC_4 = 0x25;
long NEC_5 = 0x26;
long NEC_6 = 0x27;
long NEC_7 = 0x29;
long NEC_8 = 0x2A;
long NEC_9 = 0x2B;
long NEC_0 = 0x28;

int IRledPin =  13;    // LED connected to digital pin 13 
int buttonPin = 5;      //Button connected to pin 3 
int LEDalert = 12;      //LED connected to digital pin 12 for the RTC alert 
 long int channel1 [] = {824, 9180, 4580, 500, 4580, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 4560, 520, 4560, 520, 4580, 500, 4580, 500, 31480, 9200, 2260, 520, 24644, 9180, 2280, 520};
long int channel2 [] = {35164, 9200, 4580, 500, 2280, 520, 4560, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 4580, 500, 4580, 500, 4580, 520, 34580, 9180, 2280, 520};
long int channel3 [] = {6284, 9180, 4580, 500, 4580, 520, 4560, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 4580, 500, 2280, 520, 4560, 520, 4580, 500, 31480, 9200, 2260, 520, 24644, 9200, 2260, 520};

//favchannels[] = {1,2,3};
int i=0;             //initializes the counter i to 0 
int buttonState;   //initializes the button reader variable 
 
// The setup() method runs once, when the sketch starts
 
void setup()   {                
  // initialize the IR digital pin as an output:
  pinMode(IRledPin, OUTPUT); 
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
  Wire.begin();
  irrecv.enableIRIn();
  
  
  //this initializes the RTC and a pin as the output for an LED that will alert the user 
        
  pinMode(LEDalert, OUTPUT);
  Wire.begin();       

//The values below set the state for the RTC they should be enetered when the RTC is connected  
  rtc.setClockMode(false);
  rtc.setDoW(6); //this is only a sample day for now and 1 would mean that it is Sunday
  rtc.setMonth(4); //this sets the month of the RTC to April because it is the first month 
  rtc.setDate(7); //In combination with the line above sets the RTC to the 7th of april  
  rtc.setYear(18); //This sets the year at the time the RTC is initiated not sure how the year needs to be formatted but it will not accept the full 2018 
  rtc.setHour(7);  //This sets the hour of the RTC 
  rtc.setMinute(54); 
  rtc.setSecond(00); 
}
 
void loop()                     
{
  timeCheck();
  buttonState=digitalRead(buttonPin);
  //Serial.println(buttonState);
  
  if(buttonState==LOW){
    digitalWrite(LEDalert, LOW); 
    Serial.println("In button state low");
    if(i==0){
     Serial.println("In if loop");
     irsend.sendNEC(NEC_1,32);
    /*  for(int c = 0; c<sizeof(channel1)/sizeof(long int); c++){
        Serial.println(channel1[c]);
        if(c%2==1){
          Serial.println("PulseBegin1");
          pulseIR(channel1[c]);
          Serial.println("PulseDone1");
        }
        else{
          Serial.println("Delay1");
          delayMicroseconds(channel1[c]);
        }
      }
    delayMicroseconds(1000);
    */
    
    i++;
    }
    else if(i==1){
        irsend.sendNEC(NEC_2,32);
/*      for(int c = 0; c<sizeof(channel2)/sizeof(long int); c++){
        Serial.println(c);
        Serial.println(sizeof(channel2));
        if(c%2==1){
          Serial.println("PulseBegin2");
          pulseIR(channel2[c]);
          Serial.println("PulseDone2");
        }
        else{
          Serial.println("Delay2");
          delayMicroseconds(channel2[c]);
        }
      }
    delayMicroseconds(1000);*/
    i++;
    }
    
    else { 
      irsend.sendNEC(NEC_3,32);
     /*  for(int c = 0; c<sizeof(channel3)/sizeof(long int); c++){
        Serial.println(c);
        if(c%2==1){
          Serial.println("PulseBegin3");
          pulseIR(channel3[c]);
          Serial.println("PulseDone3");
        }
        else{
          Serial.println("Delay3");
          delayMicroseconds(channel3[c]);
        }
      }
      delayMicroseconds(1000);
      */
      i=0;
    }
  }
}

 
// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}

//This method checks the time with the rtc and sets that time to a variable

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
  
  else if(h==7 && m==55 && s==00){             //This is an alert to turn on the television at 8 am to channel 5 
    digitalWrite(LEDalert, HIGH);
    Serial.println("in time alert");
  }
  else if(h==11 && m==55 && s==00){            //this is an alert to change the channel to a channel with the news at 12 TBD 
    digitalWrite(LEDalert, HIGH); 
  }
  else if(h==12 && m==25 && s==00){           //this is an alert to switch the channel back to 5 at 12:30 
    digitalWrite(LEDalert, HIGH); 
  }
  else if(h==13 && m==55 && s==00){           //this is an alert to switch the channel back to channel 4 at 2:00
    digitalWrite(LEDalert, HIGH);
  }
  else if(h==14 && m==55 && s==00 ){          //this is an alert to switch the channel back to channel 5 at 3:00 
    digitalWrite(LEDalert, HIGH); 
  }
}

