
//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code 
int IRledPin =  13;    // LED connected to digital pin 13 
int buttonPin = 3;      //Button connected to pin 3 
long int channel1 [] = {824, 9180, 4580, 500, 4580, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 4560, 520, 4560, 520, 4580, 500, 4580, 500, 31480, 9200, 2260, 520, 24644, 9180, 2280, 520};
long int channel2 [] = {35164, 9200, 4580, 500, 2280, 520, 4560, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 4580, 500, 4580, 500, 4580, 520, 34580, 9180, 2280, 520};
long int channel3 [] = {6284, 9180, 4580, 500, 4580, 520, 4560, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 4580, 500, 2280, 520, 4560, 520, 4580, 500, 31480, 9200, 2260, 520, 24644, 9200, 2260, 520};

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
      for(int c = 0; c<sizeof(channel1)/sizeof(long int); c++){
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
    i++;
    }
    else if(i==1){
      for(int c = 0; c<sizeof(channel2)/sizeof(long int); c++){
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
    i++;
    }
    
    else { 
       for(int c = 0; c<sizeof(channel3)/sizeof(long int); c++){
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
