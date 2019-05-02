/*
 * Function name: measureSimulator
 * Function inputs: takes in the initial values from init
 * Function outputs: outputs the simulated measure values.
 * Function description: runs a simulation of values that are being used currently in place of sensors,
 *                       allowing us to test our system without any real sensors.
 * Author: Haomin Yu
 */
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
void temperatureSimulator(unsigned int* tempValuePointer) {
	unsigned int tempValue = *tempValuePointer;
	static unsigned int timesCalled = 0;
	static bool increment           = false;
	timesCalled++;
	// Checking the direction of change
	if(tempValue < 15) {
		increment = true;
	}
	else if(tempValue > 50) {
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
	if(sysValue > 100) {
		*sysValuePointer = sysPressRaw_INIT;
	}
	else {
		*sysValuePointer = sysValue + delta;
	}
  if(*sysValuePointer > 100) {
    systolicMeasurementDone = true;
  }
  else {
    systolicMeasurementDone = false;
  }
}

// Simulates raw input of diastolic press
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
	if(diasValue < 40) {
		*diasValuePointer = diasPressRaw_INIT;
	}
	else {
		*diasValuePointer = diasValue + delta;
	}
  if(*diasValuePointer < 40) {
    diastolicMeasurementDone = true;
  }
  else {
    diastolicMeasurementDone = false;
  }
}

// Simulates raw input of pulse rate
void pulseRateSimulator(unsigned int* prValuePointer) {
	unsigned int prValue = *prValuePointer;
	static unsigned int timesCalled = 0;
	static bool increment           = false;
	timesCalled++;
	// Checking the direction of change
	if(prValue < 15) {
		increment = true;
	}
	else if(prValue > 40) {
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
