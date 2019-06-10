/*
 * Function name: init
 * Function inputs: it inclues stdbool and the rawstructs
 * Function outputs: it outputs all the inital values of the warnings/alarms, measurements, and computations
 * Function description: this is used to set all the main initial values for warnings/alarms, measurements, and computations.
 * Author: Haomin Yu
 *
  */
#pragma once
#include "rawStructs.h"
#include <stdbool.h>

#ifndef INIT_H_
#define INIT_H_

// Measurements
static unsigned int temperatureRawBuf_INIT[8] = {75, 0, 0, 0, 0, 0, 0, 0};
static unsigned int bloodPressRawBuf_INIT[16] = {80, 0, 0, 0, 0, 0, 0, 0,
                                                 80, 0, 0, 0, 0, 0, 0, 0};
static unsigned int pulseRateRawBuf_INIT[8]   = { 0, 0, 0, 0, 0, 0, 0, 0};
static unsigned int respirationRawBuf_INIT[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
static double ekg_real_INIT[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define SAMPLES 256             // Must be a power of 2
#define SAMPLING_FREQUENCY 9000 // Hz, must be less than 10000 due to ADC
static double ekg_imag_INIT[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Display
static unsigned short mode_INIT = 0;
static double tempCorrectedBuf_INIT[8]        = {61, 0, 0, 0, 0, 0, 0, 0};
static double bloodPressCorrectedBuf_INIT[16] = {169, 0, 0, 0, 0, 0, 0, 0,
                                                 126, 0, 0, 0, 0, 0, 0, 0};
static double pulseRateCorrectedBuf_INIT[8]   = {8, 0, 0, 0, 0, 0, 0, 0};
static double respirationCorrectedBuf_INIT[8] = {7, 0, 0, 0, 0, 0, 0, 0};
static double ekgCorrectedBuf_INIT[16] = {0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0};

// Start index of the buffers
static unsigned short currentTemperatureIndex_INIT = 0;
static unsigned short currentSysPressIndex_INIT    = 0;
static unsigned short currentDiasPressIndex_INIT   = 8;
static unsigned short currentPulseRateIndex_INIT   = 0;
static unsigned short currentRespirationIndex_INIT = 0;
static unsigned short currentEKGIndex_INIT         = 0;

// Status
static unsigned short batteryState_INIT = 200;

// Alarms
static unsigned char bpOutOfRange_INIT      = 0;
static unsigned char tempOutOfRange_INIT    = 0;
static unsigned char pulseOutOfRange_INIT   = 0;
static unsigned char batteryOutOfRange_INIT = 0;
static unsigned char respOutOfRange_INIT    = 0;

// Warning
static bool bpHigh_INIT   = false;
static bool tempHigh_INIT = false;
static bool pulseLow_INIT = false;
static bool respHigh_INIT = false;
static bool batteryLow    = false;

// TFT Keypad
static unsigned short functionSelect_INIT       = 0;
static unsigned short alarmAcknowledgement_INIT = 0;
static unsigned short localMeasurementSelection_INIT  = 0;
static unsigned short remoteMeasurementSelection_INIT = 0;

#endif /* INIT_H_ */
