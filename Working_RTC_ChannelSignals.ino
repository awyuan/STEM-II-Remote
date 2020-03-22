//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code
//The examples from https://www.arduinolibraries.info/libraries/ds3231 were very helpful with the implementation of the RTC
//The DS3132 library was used for the RTC
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

/*  WARNING (PLEASE READ !!!!!!!!):
    Even if you need to save space on your chip, do not make the following signals const arrays. We changed them to save space,
    and making them consts corrupts the arrays and makes it so that none of them work correctly.
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

/*
  PURPOSE: Initializes the infrared LED, the button, the Real Time Clock (RTC) and the SD card reader.
*/
void setup()   {
  // initialize the IR digital pin as an output:
  pinMode(IRledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
  Wire.begin();
  initializeSD();
  irrecv.enableIRIn();

  //this initializes the RTC and a pin as the output for an LED that will alert the user

  pinMode(LEDalert, OUTPUT);
  Wire.begin();

  //The values below set the state for the RTC they should be entered when the RTC is connected
  rtc.setClockMode(false);
  rtc.setDoW(6); //this is only a sample day for now and 1 would mean that it is Sunday
  rtc.setMonth(4); //this sets the month of the RTC to April because it is the first month
  rtc.setDate(7); //In combination with the line above sets the RTC to the 7th of april
  rtc.setYear(18); //This sets the year at the time the RTC is initiated not sure how the year needs to be formatted but it will not accept the full 2018
  rtc.setHour(14);  //This sets the hour of the RTC
  rtc.setMinute(54);
  rtc.setSecond(50);
}

/*
  PURPOSE: On each clock cycle, checks if the button is being pressed. We used pull up buttons, so when they are pressed
  they read as LOW and when they are read as HIGH. We chose pull up buttons because
*/
void loop()
{
  //timeCheck();
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);

  if (buttonState == LOW) {
    digitalWrite(LEDalert, LOW);
    Serial.println("In button state low");

    // sends the first channel of the favorite channels
    if (i == 0) {
      Serial.println("In if loop");
      //    irsend.sendSony(0xa90, 12);

      // turns on the sending led to indicate that a channel is being sent
      digitalWrite(LEDalert, HIGH);
      delay(100);
      digitalWrite(LEDalert, LOW);
      i++;
      
      // opens the file containing channel signals
      openFile("channels.txt");
      
    // prints the channels to the serial monitor 
    if(c<counter+1){
      for(int cmCheck = 0; cmCheck<c; cmCheck++){
        channel = atol(readLine().c_str());
        Serial.println(channel);
      }
      // this line sends the signal for the first of the favorites
      irsend.sendRaw(NEC2, sizeof(NEC2), 38);
      c++;
    }
    else{
      c=1;
    }
    closeFile();
     delay(100);
      i++;
    }
    
    // sends the second channel of the favorite channels
    else if (i == 1) {
      if(c<counter+1){
      for(int cmCheck = 0; cmCheck<c; cmCheck++){
        channel = atol(readLine().c_str());
        Serial.println(channel);
      }
      // this line sends the signal for the second of the favorites
      irsend.sendRaw(NEC5, sizeof(NEC5), 38);
      c++;
    }
    // resets the line counter
    else{
      c=1;
    }
    closeFile();
      
      delay(100);
      i++;
    }
    // sends the third channel in the favorite channels
    else {
        if(c<counter+1){
      for(int cmCheck = 0; cmCheck<c; cmCheck++){
        channel = atol(readLine().c_str());
        Serial.println(channel);
      }
      // this line sends the signal for the third of the favorites
      irsend.sendRaw(NEC7, sizeof(NEC7), 38);
      delay(100);
      i = 0;
    }
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

//PURPOSE: checks the time with the rtc and sets that time to a variable
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

  else if(h==14 && m==54 && s==50) {
    digitalWrite(LEDalert,HIGH);
    delay(10);
    digitalWrite(LEDalert,LOW);
  }

  else if(h==7 && m==54 && s==3){             //This is an alert to turn on the television at 8 am to channel 5
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

// PURPOSE: Initializes the sd card for reading and writing
void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

// Close the file!
void closeFile()
{
  if (file)
  {
    file.close();
    Serial.println("File closed");
  }
}
// Open the file for reading!
int openFile(char filename[])
{
  file = SD.open(filename);
  if (file)
  {
    Serial.println("File opened with success!");
    return 1;
  } else
  {
    Serial.println("Error opening file...");
    return 0;
  }
}
// Read a line in the text file
String readLine()
{
  String received = "";
  char ch;
  //int debug = file.available();
  //Serial.println(debug);
  while (file.available())
  {
    //Serial.println(debug);
    ch = file.read();
    //Serial.println(ch); All char are read
    if (ch == '\n')
    {
      return String(received);    
    }
    else
    {
      received += ch;
      //Serial.println(received);
    }
  }
  return "Reached End of While Loop?";
}


