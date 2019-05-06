/*
 * Function name: compute
 * Function inputs: inputs te raw measure data tp be computed
 * Function outputs: outputs the computed data values
 * Function description: takes the sensor measured data and using their corresponding units to calculate the actual
 *                       unit value of the measured data.
 * Author: Haomin Yu
 */
#include "rawStructs.h"
#include <stdio.h>

// Function prototypes
void compute(void* Data);
double computeTemp(unsigned int* tempRaw);
double computeSys(unsigned int* systolicRaw);
double computeDias(unsigned int* diastolicRaw);
double computePr(unsigned int* bpRaw);

// Global variable
bool enableCompute = true;

// Computes the values for 'tempCorrected', 'sysCorrected',
// 'diasCorrected', and 'prCorrected'
void compute(void* Data) {
  if(enableCompute) {
  	ComputeDataStruct data = *((ComputeDataStruct*)Data);
  	*data.tempCorrected      = computeTemp(data.temperatureRaw);
  	*data.sysPressCorrected  = computeSys(data.systolicPressRaw);
  	*data.diasPressCorrected = computeDias(data.diastolicPressRaw);
  	*data.prCorrected        = computePr(data.pulseRateRaw);
  }
}

// Computes the temperature in celsius
double computeTemp(unsigned int* tempRaw) {
	return 5 + 0.75*(*tempRaw);
}

// Computes the systolic pressure in mmHg
double computeSys(unsigned int* systolicRaw) {
	return 9 + 2*(*systolicRaw);
}

// Computes the diastolic pressure in mmHg
double computeDias(unsigned int* diastolicRaw) {
  return 6 + 1.5*(*diastolicRaw);
}

// Computes the pulse rate in beats per minute
double computePr(unsigned int* bpRaw) {
	return 8 + 3*(*bpRaw);
}
