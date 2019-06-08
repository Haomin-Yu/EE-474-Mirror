/*
 * Function name: compute
 * Function inputs: inputs te raw measure data tp be computed
 * Function outputs: outputs the computed data values
 * Function description: takes the sensor measured data and using their corresponding units to calculate the actual
 *                       unit value of the measured data.
 * Author: Haomin Yu
 */
#include "init.h"
#include "rawStructs.h"
#include <stdio.h>

// Function prototypes
void compute(void* Data);
double computeTemp(unsigned int tempRaw);
double computeSys(unsigned int systolicRaw);
double computeDias(unsigned int diastolicRaw);
double computePr(unsigned int bpRaw);
double computeRespiration(unsigned int respirationRaw);

// Getting global indicators from measure.ino
extern bool tempRawChanged;
extern bool bloodPressureRawChanged;
extern bool respirationRawChanged;
extern bool pulseRateRawChanged;
extern bool ekgRawChanged;

// Global variables
// (Indicating whether a new computation has been made)
bool newTempComputed;
bool newBloodPressComputed;
bool newPulseRateComputed;
bool newRespirationComputed;
bool newEKGComputed;

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
   if(bloodPressureRawChanged) {
     unsigned short index = *data.currentSysPressIndex;
     data.bloodPressCorrectedBuf[index] = computeSys(data.bloodPressRawBuf[index]);
     index = *data.currentDiasPressIndex;
     data.bloodPressCorrectedBuf[index] = computeDias(data.bloodPressRawBuf[index]);
     bloodPressureRawChanged = false;
     newBloodPressComputed   = true;
   }
   if(respirationRawChanged) {
     unsigned short index = *data.currentRespirationIndex;
     data.respirationRateCorrectedBuf[index] = computeRespiration(data.respirationRateRawBuf[index]);
     respirationRawChanged  = false;
     newRespirationComputed = true;
   }
   if(pulseRateRawChanged) {
     unsigned short index = *data.currentPulseRateIndex;
     data.prCorrectedBuf[index] = computePr(data.pulseRateRawBuf[index]);
     pulseRateRawChanged  = false;
     newPulseRateComputed = true;
   }
   if(ekgRawChanged) {
     unsigned short nextIndex = (*data.currentEKGIndex + 1) % 16;
     data.EKGFreqBuf[nextIndex] = computeEKG(data.EKGRawBuf);
     *data.currentEKGIndex = nextIndex;
     ekgRawChanged  = false;
     newEKGComputed = true;
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

// Computes the diastolic pressure in mm
double computeDias(unsigned int diastolicRaw) {
  return 6 + 1.5*diastolicRaw;
}

// Computes the respiration in breaths per minute
double computeRespiration(unsigned int respirationRaw) {
  //return 7 + 3*respirationRaw;
  return respirationRaw*10;
}

// Computes the pulse rate in beats per minute
double computePr(unsigned int bpRaw) {
	//return 8 + 3*bpRaw;
  return bpRaw*10;
}

// Computes the peak EKG from the measurements
double computeEKG(double* arrayPointer) {
  FFT.Windowing(arrayPointer, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(arrayPointer, ekg_imag_INIT, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(arrayPointer, ekg_imag_INIT, SAMPLES);
  double peak = FFT.MajorPeak(arrayPointer, SAMPLES, SAMPLING_FREQUENCY);
  return ekgNormalizer(peak);
}
