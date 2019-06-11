/*
 * Function name: measureSimulator
 * Function inputs: takes in the initial values from init (THIS IS NO LONGER USED BECAUSE WE NO LONGER SIMULATE MEASUREMENTS)
 * Function outputs: outputs the simulated measure values.
 * Function description: runs a simulation of values that are being used currently in place of sensors,
 *                       allowing us to test our system without any real sensors.
 * Author: Haomin Yu
 * 
 *** (This class is deprecated as of Project 4) ***
 */

/********** (Deprecated Class) **********
#include "init.h"
#include <stdbool.h>

// Function prototypes
void temperatureSimulator(unsigned int* tempValuePointer);
void systolicPressSimulator(unsigned int* sysValuePointer);
void diastolicPressSimulator(unsigned int* diasValuePointer);
void pulseRateSimulator(unsigned int* prValuePointer);
// Global indicators
bool systolicMeasurementDone  = false;
bool diastolicMeasurementDone = false;

// Simulates raw input of temperature
// (This function is deprecated as of Project 4)
static const unsigned int temperatureMin = 15;
static const unsigned int temperatureMax = 50;
void temperatureSimulator(unsigned int* tempValuePointer) {
	unsigned int tempValue = *tempValuePointer;
	static unsigned int timesCalled = 0;
	static bool increment           = false;
	timesCalled++;
	// Checking the direction of change
	if(tempValue < temperatureMin) {
		increment = true;
	}
	else if(tempValue > temperatureMax) {
		increment = false;
	}
	// Calculating the change in value
	int delta;
	if(timesCalled % 2 == 0) {
		delta = 2;
	}
	else {
		delta = -1;
	}
	if(!increment) {
		delta = -delta;
	}
	*tempValuePointer = tempValue + delta;
}

// Simulates raw input of systolic press
// (This function is deprecated as of Project 4)
static const unsigned int systolicPressThreshold = 100;
void systolicPressSimulator(unsigned int* sysValuePointer) {
	unsigned int sysValue = *sysValuePointer;
	static unsigned int timesCalled = 0;
	timesCalled++;
	int delta;
	if(timesCalled % 2 == 0) {
		delta = 3;
	}
	else {
		delta = -1;
	}
	if(sysValue > systolicPressThreshold) {
		*sysValuePointer = sysPressRaw_INIT;
	}
	else {
		*sysValuePointer = sysValue + delta;
	}
  if(*sysValuePointer > systolicPressThreshold) {
    systolicMeasurementDone = true;
  }
  else {
    systolicMeasurementDone = false;
  }
}

// Simulates raw input of diastolic press
// (This function is deprecated as of Project 4)
static const unsigned int diastolicPressThreshold = 40;
void diastolicPressSimulator(unsigned int* diasValuePointer) {
	unsigned int diasValue = *diasValuePointer;
	static unsigned int timesCalled = 0;
	timesCalled++;
	int delta;
	if(timesCalled % 2 == 0) {
		delta = -2;
	}
	else {
		delta = 1;
	}
	if(diasValue < diastolicPressThreshold) {
		*diasValuePointer = diasPressRaw_INIT;
	}
	else {
		*diasValuePointer = diasValue + delta;
	}
  if(*diasValuePointer < diastolicPressThreshold) {
    diastolicMeasurementDone = true;
  }
  else {
    diastolicMeasurementDone = false;
  }
}

// Simulates raw input of pulse rate
// (This function is deprecated as of Project 3)
static const unsigned int pulseRateMin = 15;
static const unsigned int pulseRateMax = 40;
void pulseRateSimulator(unsigned int* prValuePointer) {
	unsigned int prValue = *prValuePointer;
	static unsigned int timesCalled = 0;
	static bool increment           = false;
	timesCalled++;
	// Checking the direction of change
	if(prValue < pulseRateMin) {
		increment = true;
	}
	else if(prValue > pulseRateMax) {
		increment = false;
	}
	// Calculating the change in value
	int delta;
	if(timesCalled % 2 == 0) {
		delta = -1;
	}
	else {
		delta = 3;
	}
	if(!increment) {
		delta = -delta;
	}
	*prValuePointer = prValue + delta;
}

********** (Deprecated Class) **********/
