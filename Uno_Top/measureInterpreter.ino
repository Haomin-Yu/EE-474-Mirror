/* 
 * Function name: measureInterpreter
 * Function inputs: takes an ac wave input representing pulse rate or resperation. It also takes in signals from
 * buttons and a potentiometer for other measurements.
 * Function outputs: outputs the pointer value corresponding to the reequested measurement
 * Function description: this is used in order to calculate all of the measurements requested by the
 * mega.
 * 
 * Author: Nathan Ness, Haomin Yu
 */
#include "init.h"

// Function prototypes
void pulseInterupt();
void respirationInterupt();
void temperatureInterpreter(unsigned int* tempValuePointer);
void systolicPressInterpreter(unsigned int* sysValuePointer);
void diastolicPressInterpreter(unsigned int* diasValuePointer);
void pulseRateInterpreter(unsigned int* prValuePointer);
void respirationRateInterpreter(unsigned int* respirationValuePointer);

// Class constants
static const double MINUTE = 60000.0;
static const unsigned int ANALOG_RESOLUTION = 1023;
static const unsigned int  MIN_TEMPERATURE = 0;
static const unsigned int  MAX_TEMPERATURE = 115;
static const unsigned int  MIN_SYS_PRESS   = 50;
static const unsigned int  MAX_SYS_PRESS   = 71;
static const unsigned int  MIN_DIAS_PRESS  = 29;
static const unsigned int  MAX_DIAS_PRESS  = 49;
static const unsigned int PULSE_THRESHOLD       = 5;  //constant used to say what voltage represents a high signal
static const unsigned int RESPIRATION_THRESHOLD = 7;
static const unsigned int LOGICAL_THRESHOLD     = ANALOG_RESOLUTION - 1;

// Class variable
static unsigned long pulseCount       = 0;
static unsigned long respirationCount = 0;

void pulseInterupt() { //if interupt is called it will increment counter
  pulseCount++;
}

void respirationInterupt() {//if interupt is called it will increment counter
  respirationCount++;
}

// Interprets signal from TEMP_ANALOG_IN as actual temperature
static const double TEMP_RANGE = MAX_TEMPERATURE - MIN_TEMPERATURE;
void temperatureInterpreter(unsigned int* tempValuePointer) {
  unsigned int tempRead = analogRead(TEMP_ANALOG_IN);
  *tempValuePointer = MIN_TEMPERATURE + tempRead * TEMP_RANGE / ANALOG_RESOLUTION;
}

// Interprets signals from BUTTON_ANALOG_IN and SWITCH_ANALOG_IN and
// retrieves a number within MIN_SYS_PRESS and MAX_SYS_PRESS
static bool prevBloodPressureOn = false;
void systolicPressInterpreter(unsigned int* bloodPressurePointer) {
  prevBloodPressureOn     = false;
  while(!(*bloodPressurePointer >= MIN_SYS_PRESS && *bloodPressurePointer <= MAX_SYS_PRESS)) {
    if(analogRead(BUTTON_ANALOG_IN) > LOGICAL_THRESHOLD) {
      delay(50);
      if(!prevBloodPressureOn) {
        if(analogRead(SWITCH_ANALOG_IN) > LOGICAL_THRESHOLD) {
          *bloodPressurePointer *= 1.1;
        }
        else {
          *bloodPressurePointer /= 1.1;
        }
      }
      prevBloodPressureOn = true;
    }
    else {
      prevBloodPressureOn = false;
    }
  }
}

// Interprets signals from BUTTON_ANALOG_IN and SWITCH_ANALOG_IN and
// retrieves a number within MIN_DIAS_PRESS and MAX_DIAS_PRESS
void diastolicPressInterpreter(unsigned int* bloodPressurePointer) {
  prevBloodPressureOn = false;
  while(!(*bloodPressurePointer >= MIN_DIAS_PRESS && *bloodPressurePointer <= MAX_DIAS_PRESS)) {
    if(analogRead(BUTTON_ANALOG_IN) > LOGICAL_THRESHOLD) {
      delay(50);
      if(!prevBloodPressureOn) {
        if(analogRead(SWITCH_ANALOG_IN) > LOGICAL_THRESHOLD) {
          *bloodPressurePointer *= 1.1;
        }
        else {
          *bloodPressurePointer /= 1.1;
        }
      }
      prevBloodPressureOn = true;
    }
    else {
      prevBloodPressureOn = false;
    }
  }
}

// Interprets signal from PULSE_ANALOG_IN as actial pulse rates
static const unsigned long MEASURE_PULSE_TIME = 6000; //Decides how long the pulse will be measured
void pulseRateInterpreter(unsigned int* prValuePointer) {
  unsigned long startTime = millis();
  pulseCount = 0;                                                                   //resets counter value
  attachInterrupt(digitalPinToInterrupt(PULSE_INTERRUPT), pulseInterupt, FALLING);  //attaches inturpt when it sees a falling edge
  while (millis() - startTime < MEASURE_PULSE_TIME) {                               //runs for 6 seconds
    if(analogRead(PULSE_ANALOG_IN) < PULSE_THRESHOLD) {                             //decides if it will be a low or high signal
      digitalWrite(PULSE_DIGITAL_OUT, LOW);
    } else {
      digitalWrite(PULSE_DIGITAL_OUT, HIGH);
    }
  }
  detachInterrupt(PULSE_INTERRUPT);                                                 //detaches interupt
  *prValuePointer = pulseCount;
}

// Interprets signal from RESPIRATION_ANALOG_IN as actial respiration rates
static const unsigned long MEASURE_RESPIRATION_TIME = 6000;//Defines how long resperation is measured
void respirationRateInterpreter(unsigned int* respirationValuePointer) {
  unsigned long startTime = millis();
  respirationCount = 0;                                                                         //resets counter value
  attachInterrupt(digitalPinToInterrupt(RESPIRATION_INTERRUPT), respirationInterupt, FALLING);  //attaches inturpt when it sees a falling edge
  while (millis() - startTime < MEASURE_RESPIRATION_TIME) {                                     //runs for 6 seconds
    if(analogRead(RESPIRATION_ANALOG_IN) < RESPIRATION_THRESHOLD) {                             //decides if it will be a low or high signal
      digitalWrite(RESPIRATION_DIGITAL_OUT, LOW);
    } else {
      digitalWrite(RESPIRATION_DIGITAL_OUT, HIGH);
    }
  }
  detachInterrupt(RESPIRATION_INTERRUPT);
  *respirationValuePointer = respirationCount;
}
