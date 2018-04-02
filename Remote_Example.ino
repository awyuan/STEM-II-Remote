//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code 
int IRledPin =  13;    // LED connected to digital pin 13 
int buttonPin = 3;      //Button connected to pin 3 
int channel1[]={740000,168000,740000}; //Channel Example
int channel2[]={800000,100000,400000,100000,300000}; //Channel Example 2
int channel3[]={600000,1000000,500000,100000,800000}; //Channel Example 3

int i=0;             //initializes the counter i to 0 
int buttonState;   //initializes the button reader variable 
 
// The setup() method runs once, when the sketch starts
 
void setup()   {                
  // initialize the IR digital pin as an output:
  pinMode(IRledPin, OUTPUT); 
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
}
 
void loop()                     
{
buttonState=digitalRead(buttonPin);
  Serial.println(buttonState);
  if(buttonState==LOW){
    Serial.println("In button state low");
    if(i==0){
     Serial.println("In if loop");
      for(int c = 0; c<sizeof(channel1); c++){
        Serial.println(c);
        if(c%2==0){
          Serial.println("PulseBegin1");
          pulseIR(channel1[c]);
          Serial.println("PulseDone1");
        }
        else{
          Serial.println("Delay1");
          delayMicroseconds(channel1[c]);
        }
      }
    delay(400);
    i++;
    }
    else if(i==1){
      for(int c = 0; c<sizeof(channel2); c++){
        Serial.println(c);
        Serial.println(sizeof(channel2));
        if(c%2==0){
          Serial.println("PulseBegin2");
          pulseIR(channel2[c]);
          Serial.println("PulseDone2");
        }
        else{
          Serial.println("Delay2");
          delayMicroseconds(channel2[c]);
        }
      }
    delay(400);
    i++;
    }
    
    else { 
       for(int c = 0; c<sizeof(channel3); c++){
        Serial.println(c);
        if(c%2==0){
          Serial.println("PulseBegin3");
          pulseIR(channel3[c]);
          Serial.println("PulseDone3");
        }
        else{
          Serial.println("Delay3");
          delayMicroseconds(channel3[c]);
        }
      }
      delay(400);
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
