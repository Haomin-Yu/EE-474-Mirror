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
     unsigned short index = (*data.nextTemperatureIndex == 0)? 
                            7: (*data.nextTemperatureIndex - 1);
     data.tempCorrectedBuf[index] = computeTemp(data.temperatureRawBuf[*data.nextTemperatureIndex]);
     tempRawChanged  = false;
     newTempComputed = true;
   }
   if(sysPressRawChanged) {
     unsigned short index = (*data.nextSysPressIndex == 0)? 
                            7: (*data.nextSysPressIndex - 1);
     
     sysPressRawChanged  = false;
     newSysPressComputed = true;
   }
   if(diasPressRawChanged) {
     unsigned short index = (*data.nextDiasPressIndex == 0)? 
                            7: (*data.nextDiasPressIndex - 1);
     
     diasPressRawChanged = false;
     newSysPressComputed = true;
   }
   if(pulseRateRawChanged) {
     unsigned short index = (*data.nextPulseRateIndex == 0)? 
                            7: (*data.nextPulseRateIndex - 1);
     data.prCorrectedBuf[index] = computePr(data.pulseRateRawBuf[*data.nextPulseRateIndex]);
     pulseRateRawChanged = false;
     newSysPressComputed = true;
   }

   bloodPressRawBuf
   bloodPressCorrectedBuf
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
