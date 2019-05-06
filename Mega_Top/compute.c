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
double computeTemp(unsigned int tempRaw);
double computeSys(unsigned int systolicRaw);
double computeDias(unsigned int diastolicRaw);
double computePr(unsigned int bpRaw);

// Getting global indicators from measure.ino
extern bool tempRawChanged;
extern bool sysPressRawChanged;
extern bool diasPressRawChanged;
extern bool pulseRateRawChanged;

// Global variables
// (Indicating whether a new computation has been made)
bool newTempComputed;
bool newSysPressComputed;
bool newDiasPressComputed;
bool newPulseRateComputed;

// Computes the values for 'tempCorrected', 'sysCorrected',
// 'diasCorrected', and 'prCorrected' if needed
void compute(void* Data) {
   ComputeDataStruct data = *((ComputeDataStruct*)Data);
   if(tempRawChanged) {
     unsigned short index = *data.currentTemperatureIndex;
     data.tempCorrectedBuf[index] = computeTemp(data.temperatureRawBuf[index]);
     tempRawChanged  = false;
     newTempComputed = true;
   }
   if(sysPressRawChanged) {
     unsigned short index = *data.currentSysPressIndex;
     data.bloodPressCorrectedBuf[index] = computeSys(data.bloodPressRawBuf[index]);
     sysPressRawChanged  = false;
     newSysPressComputed = true;
   }
   if(diasPressRawChanged) {
     unsigned short index = *data.currentDiasPressIndex;
     data.bloodPressCorrectedBuf[index] = computeDias(data.bloodPressRawBuf[index]);
     diasPressRawChanged = false;
     newDiasPressComputed = true;
   }
   if(pulseRateRawChanged) {
     unsigned short index = *data.currentPulseRateIndex;
     data.prCorrectedBuf[index] = computePr(data.pulseRateRawBuf[index]);
     pulseRateRawChanged = false;
     newSysPressComputed = true;
   }
}

// Computes the temperature in celsius
double computeTemp(unsigned int tempRaw) {
	return 5 + 0.75*tempRaw;
}

// Computes the systolic pressure in mmHg
double computeSys(unsigned int systolicRaw) {
	return 9 + 2*systolicRaw;
}

// Computes the diastolic pressure in mmHg
double computeDias(unsigned int diastolicRaw) {
  return 6 + 1.5*diastolicRaw;
}

// Computes the pulse rate in beats per minute
double computePr(unsigned int bpRaw) {
	return 8 + 3*bpRaw;
}
