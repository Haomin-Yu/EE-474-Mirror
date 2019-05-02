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
static unsigned int temperatureRaw_INIT = 75;
static unsigned int sysPressRaw_INIT    = 80;
static unsigned int diasPressRaw_INIT   = 80;
static unsigned int pulseRateRaw_INIT   = 50;

// Display
static double tempCorrected_INIT      = 0.0;
static double sysPressCorrected_INIT  = 0.0;
static double diasPressCorrected_INIT = 0.0;
static double pulseRateCorrected_INIT = 0.0;

// Status
static unsigned short batteryState_INIT = 200;

// Alarms
static unsigned char bpOutOfRange_INIT      = 0;
static unsigned char tempOutOfRange_INIT    = 0;
static unsigned char pulseOutOfRange_INIT   = 0;
static unsigned char batteryOutOfRange_INIT = 0;

// Warning
static bool bpHigh_INIT   = false;
static bool tempHigh_INIT = false;
static bool pulseLow_INIT = false;
static bool batteryLow    = false;

#endif /* INIT_H_ */
