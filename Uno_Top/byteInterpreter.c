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
 * (Others will do nothing)
 */
#include "init.h"
#include <stdbool.h>

// Getting the global indicators
extern bool systolicMeasurementDone;
extern bool diastolicMeasurementDone;

// Storing previous data for simulation of measurements
static unsigned int* tempValuePointer = &temperatureRaw_INIT;
static unsigned int* sysValuePointer  = &sysPressRaw_INIT;
static unsigned int* diasValuePointer = &diasPressRaw_INIT;
static unsigned int* prValuePointer   = &pulseRateRaw_INIT;
// Grabbing Simulation Functions
extern void temperatureSimulator(unsigned int* tempValuePointer);
extern void systolicPressSimulator(unsigned int* sysValuePointer);
extern void diastolicPressSimulator(unsigned int* diasValuePointer);
extern void pulseRateSimulator(unsigned int* prValuePointer);

// Interprets the byte as described in class header
unsigned int interpretByte(unsigned char input);
unsigned int interpretByte(unsigned char input) {
   switch(input) {
      case 0x00:
         temperatureSimulator(tempValuePointer);
         return *tempValuePointer;
         break;
      case 0x01:
         while(!systolicMeasurementDone) {
            systolicPressSimulator(sysValuePointer);
         }
         return *sysValuePointer;
         break;
      case 0x02:
         while(!diastolicMeasurementDone) {
            diastolicPressSimulator(diasValuePointer);
         }
         return *diasValuePointer;
         break;
      case 0x03:
         pulseRateSimulator(prValuePointer);
         return *prValuePointer;
         break;
      default:
         return 0;
         break;
  }
}
