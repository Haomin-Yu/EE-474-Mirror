/*
 * Function name: byteinterpreter
 * Function inputs: it takes in the variables systolicMeasurementDone and diastolicMeasurementDone. 
 *                  As well as sensor simulators and the command from the mega.
 * Function outputs: it outputs the information that is measured based off the command it got from the mega.
 * Function description: Translates the bytes its recieving from the mega and uses it to decide what measurement
 *                       action it is meant to take.
 * Author: Haomin Yu
 *****************************************
 * 0x00: Measure Temperature
 * 0x01: Measure Systolic Pressure
 * 0x02: Measure Diastolic Pressure
 * 0x03: Measure Pulse Rate
 * 0x04: Measure Respiration Rate
 * (Others will do nothing)
 */
#include "init.h"
#include "measureInterpreter.h"
#include "byteInterpreter.h"
#include <stdbool.h>

// Getting the global indicators
extern bool systolicMeasurementDone;
extern bool diastolicMeasurementDone;

// Storing previous data for simulation of measurements
static unsigned int* tempValuePointer     = &temperatureRaw_INIT;
       unsigned int* bloodPressurePointer = &bloodPressureRaw_INIT;
static unsigned int* prValuePointer       = &pulseRateRaw_INIT;
static unsigned int* respirationValuePointer = &respirationRateRaw_INIT;
// Grabbing external Functions
extern void temperatureInterpreter(unsigned int* tempValuePointer);
extern void systolicPressInterpreter(unsigned int* sysValuePointer);
extern void diastolicPressInterpreter(unsigned int* diasValuePointer);
extern void pulseRateInterpreter(unsigned int* prValuePointer);
extern void respirationRateInterpreter(unsigned int* respirationValuePointer);

// Interprets the byte as described in class header
unsigned int interpretByte(unsigned char input);
unsigned int interpretByte(unsigned char input) {
   switch(input) {
      case measureTemperature:
         temperatureInterpreter(tempValuePointer);
         return *tempValuePointer;
         break;
      case measureBloodPressure:
         if(*bloodPressurePointer == bloodPressureRaw_INIT) {
           systolicPressInterpreter(bloodPressurePointer);
           return *bloodPressurePointer;
         }
         else {
           diastolicPressInterpreter(bloodPressurePointer);
           return *bloodPressurePointer;
         }
         break;
      case measurePulseRate:
         pulseRateInterpreter(prValuePointer);
         return *prValuePointer;
         break;
      case measureRespiration:
         respirationRateInterpreter(respirationValuePointer);
         return *respirationValuePointer;
         break;
      default:
         return 0;
         break;
  }
}
