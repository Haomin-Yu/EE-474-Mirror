/* 
 * (Type shit here)
 * 
 * Author: Nathan Ness
 */

static const unsigned long COUNTER = 0;
 
 unsigned long pulseInterupt(){
  COUNTER++;
 }




// Simulates raw input of pulse rate
// (This function is deprecated as of Project 3)

void pulseRateSimulator(unsigned int* prValuePointer) {
  unsigned long startTime = millis();
  COUNTER = 0;
  unsigned int analogIn;
  attachInterrupt(digitalPinToInterrupt(2), pulseInterupt, FALLING);
  while (millis() - startTime < 6000) {
    analogIn = analogRead(4);
    if(analogIn < 5) {
      digitalWrite(13, LOW);
    } else {
      digitalWrite(13, HIGH);
    }
  }
  detachInterrupt(2);
  counter = counter * 10;
  *prValuePointer = counter;
}
