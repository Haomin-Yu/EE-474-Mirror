/* 
 * (Type shit here)
 * 
 * Author: Nathan Ness
 */
// Pin assignments
static const int INTERRUPT     = 2;
static const int PULSE_DIGITAL = 13;
static const int PULSE_IN      = 4;
// Class variable
static unsigned long COUNTER = 0;
 
unsigned long pulseInterupt(){
  COUNTER++;
}

// Simulates raw input of pulse rate
// (This function is deprecated as of Project 3)
static const unsigned int PULSE_THRESHOLD = 5;
void pulseRateSimulator(unsigned int* prValuePointer) {
  unsigned long startTime = millis();
  COUNTER = 0;
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), pulseInterupt, FALLING);
  while (millis() - startTime < 6000) {
    if(analogRead(PULSE_IN) < PULSE_THRESHOLD) {
      digitalWrite(PULSE_DIGITAL, LOW);
    } else {
      digitalWrite(PULSE_DIGITAL, HIGH);
    }
  }
  detachInterrupt(INTERRUPT);
  *prValuePointer = COUNTER * 10;
}
