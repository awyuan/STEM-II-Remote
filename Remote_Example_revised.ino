//Modified the starting code of a button from http://www.instructables.com/id/How-to-control-your-TV-with-an-Arduino/
//Tutorials and examples from https://www.arduino.cc/en/Tutorial/Button were used to help with the code
int IRledPin =  13;    // LED connected to digital pin 13
int buttonPin = 3;      //Button connected to pin 3
long int channel1 [] = {824, 9180, 4580, 500, 4580, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 4560, 520, 4560, 520, 4580, 500, 4580, 500, 31480, 9200, 2260, 520, 24644, 9180, 2280, 520};
long int channel2 [] = {35164, 9200, 4580, 500, 2280, 520, 4560, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2300, 500, 2280, 520, 2280, 500, 2280, 520, 4580, 500, 4580, 500, 4580, 520, 34580, 9180, 2280, 520};
long int channel3 [] = {6284, 9180, 4580, 500, 4580, 520, 4560, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 520, 2260, 520, 2280, 520, 2280, 500, 2280, 520, 2280, 500, 2300, 500, 4580, 500, 2280, 520, 4560, 520, 4580, 500, 31480, 9200, 2260, 520, 24644, 9200, 2260, 520};

int i = 0;           //initializes the counter i to 0
int buttonState;   //initializes the button reader variable

// The setup() method runs once, when the sketch starts

/*
  PURPOSE: Initializes the pin for the infrared led and one button.
*/
void setup()   {
  // initialize the IR digital pin as an output:
  pinMode(IRledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
}

/*
  PURPOSE: On each clock cycle, checks whether the button to change channels is being pressed.
  If the button is pressed, the next channel signal is sent using the IR LED.
*/
void loop()
{
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if (buttonState == LOW) {
    Serial.println("In button state low");
    /* i is the counter variable, which tracks which channel will be sent.
      i = 0 -> send channel 1, i = 1 -> send channel 2, i = 2 -> sned channel 3*/
    if (i == 0) {
      sendChannel(channel1);
      i = i + 1;
    }
    else if (i == 1) {
      sendChannel(channel2);
      i = i + 1;
    }

    else {
      sendChannel(channel1);
      i = 0;
    }
  }
}



/*
 * PURPOSE:
 * This procedure sends a 38KHz pulse to the IRledPin
 * for a certain # of microseconds. We'll use this whenever we need to send codes.
 */
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

/*
 * PURPOSE: Sends the given channel signal using the infrared LED.
 * The channel signal is represented by an array of integers.
 * The integer e at each even index represents a delay which is e microseconds long.
 * The integer o at each odd index represents a 38KHz pulse which is o microseconds long.
 * The alternating delays and pulses produce a specific signal for the remote.
 */
void sendChannel(long int channel []) {
  for (int c = 0; c < sizeof(channel) / sizeof(long int); c++) {
    Serial.println(c);
    Serial.println(sizeof(channel));
    if (c % 2 == 1) {
      pulseIR(channel[c]);
    }
    else {
      delayMicroseconds(channel[c]);
    }
  }
  delayMicroseconds(1000);
}
