//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code
//The examples from https://www.arduinolibraries.info/libraries/ds3231 were very helpful with the implementation of the RTC
//The DS3132 library was used for the RTC 
#include <Wire.h>

int buttonPin = A3;     // Channels
int buttonPin1 = 13;    // Volume Up 
int buttonPin2 = 2;     // Volume Down
int buttonPin3 = A0;    // On/Off
int buttonPin4 = 12;    // OnDemand
int buttonPin5 = 5;     // Extra

int buttonState;   //initializes the button reader variable
int buttonState1;
int buttonState2;
int buttonState3;
int buttonState4;
int buttonState5;

/* The setup() method runs once, when the sketch starts
 PURPOSE: initalizes the pins on the Arduino which the button remotes were wired to
 They are all set as INPUT so that digitalRead() can be called to get their voltage.
 The buttons are all set to HIGH because we used pull up buttons, which should be HIGH
 by default. This is because pull up buttons produce voltage when not pressed, and
 halt voltage when being pressed.
*/
void setup()   {
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(buttonPin1, HIGH);
  digitalWrite(buttonPin2, HIGH);
  digitalWrite(buttonPin3, HIGH);
  digitalWrite(buttonPin4, HIGH);
  digitalWrite(buttonPin5, HIGH);
  Serial.begin(9600);
  Wire.begin();  
}

/*
  PURPOSE: Reads the button state at each cycle. If the digitalRead() returns LOW,
  that means the button is being pressed. Remember, this is because we used pull
  up buttons, which are HIGH when being pressed and LOW when released. If you use,
  standard buttons, this would be flipped. We used pull-up buttons so that our remote
  would not be susceptible to false positives from background signals.
*/
void loop()
{
  buttonState = digitalRead(buttonPin);
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);

  //Serial.println(buttonState);

  // If the button is pressed, enter the channel loop
  if (buttonState == LOW) {
    Serial.println("Button Works");
  }
  if (buttonState1 == LOW) {
    Serial.println("Button1 Works");
  }
  if (buttonState2 == LOW) {
    Serial.println("Button2 Works");
  }
  if (buttonState3 == LOW) {
    Serial.println("Button3 Works");
  }
  if (buttonState4 == LOW) {
    Serial.println("Button4 Works");
  }
  if (buttonState5 == LOW) {
    Serial.println("Button5 Works");
  }
}

