/* 
 *Function name: measureInterpreter
 * Function inputs: takes an ac wave input representing pulse rate
 * Function outputs: outputs the pointer value corresponding to the measured pulse rate value
 * Function description: this is used in order to calculate the pulserate from an inputed ac voltage function.
 * 
 * Author: Nathan Ness
 */
// Function prototypes
void pulseInterupt();
void pulseRateInterpreter(unsigned int* prValuePointer);

// Pin assignments
static const int INTERRUPT     = 2;
static const int PULSE_DIGITAL = 13;
static const int PULSE_IN      = A0;

// Class variable
static unsigned long COUNTER = 0;
 
void pulseInterupt() {//if interupt is called it will increment counter
  COUNTER++;
}

// Simulates raw input of pulse rate
// (This function is deprecated as of Project 3)
static const unsigned int PULSE_THRESHOLD = 5;    //constant used to say what voltage represents a high signal
void pulseRateInterpreter(unsigned int* prValuePointer) {
  unsigned long startTime = millis();
  COUNTER = 0;                                                                      //resets counter value
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), pulseInterupt, FALLING);        //attaches inturpt when it sees a falling edge
  while (millis() - startTime < 6000) {                                             //runs for 6 seconds
    if(analogRead(A0) < PULSE_THRESHOLD) {                                          //decides if it will be a low or high signal
      digitalWrite(PULSE_DIGITAL, LOW);
    } else {
      digitalWrite(PULSE_DIGITAL, HIGH);
    }
  }
  detachInterrupt(INTERRUPT);                                                       //detaches interupt
  *prValuePointer = COUNTER * 10;                                                   //multiplies the measured data by 10 to make it beats per minute.
}
