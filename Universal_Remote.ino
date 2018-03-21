//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code 
int IRledPin =  13;    // LED connected to digital pin 13 
int buttonPin = 3;      //Button connected to pin 3 
int delayNum[]={790,1680,740}; //initializes the array of the delay numbers --> these numbers are currently place holders 
int pulseNum[]={450,760,940}; //initializes the array of the pulse numbers --> these numbers are currently place holders 


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

if(buttonState==LOW){
if(i<sizeof(delayNum)-1){
  delayMicroseconds(delayNum[i]); //Time off (Left Column on serial monitor)
  pulseIR(pulseNum[i]);           //Time on  (Right Column on serial monitor)
    delay(10000);  // wait twenty seconds (20 seconds * 1000 milliseconds)
    i++;
}
 
 else{ 
  delayMicroseconds(delayNum[i]); //Time off (Left Column on serial monitor)
  pulseIR(pulseNum[i]);           //Time on  (Right Column on serial monitor)
  delay(10000);  // wait twenty seconds (20 seconds * 1000 milliseconds)
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
