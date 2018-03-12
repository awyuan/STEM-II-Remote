//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
int IRledPin =  13;    // LED connected to digital pin 13
int delayNum[]={1660,1640}; //These are just testing values 
int pulseIRNum[]={580,560}; //These are just tesing values 
 
// The setup() method runs once, when the sketch starts
 
void setup()   {                
  // initialize the IR digital pin as an output:
  pinMode(IRledPin, OUTPUT);      
 
  Serial.begin(9600);
}
 
void loop()                     
{

 //Also if the void loop does not keep going then at the end there should be an if (i==sizeof(delayNum)-1) i=0 so that it goes back to the original 
 //The variable i should change when the button is pressed but I don't know how to make this happen 
  for(int i=0; i<sizeof(delayNum); i++){
  delayMicroseconds(delayNum[i]); //Time off (Left Column on serial monitor)
  pulseIR(pulseIRNum[i]);           //Time on  (Right Column on serial monitor)
  
  delay(10000);  // wait twenty seconds (20 seconds * 1000 milliseconds)
  }
