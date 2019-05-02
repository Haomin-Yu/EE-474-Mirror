/*
 * Function name: init
 * Function inputs: it inclues stdbool
 * Function outputs: it outputs all the inital values of the warnings/alarms, measurements, and computations
 * Function description: this is used to set all the main initial values for warnings/alarms, measurements, and computations.
 * Author: Haomin Yu
 */
#pragma once
#include <stdbool.h>

#ifndef INIT_H_
#define INIT_H_

// Measurements initial values
static unsigned int temperatureRaw_INIT = 75;
static unsigned int sysPressRaw_INIT    = 80;
static unsigned int diasPressRaw_INIT   = 80;
static unsigned int pulseRateRaw_INIT   = 50;

// Display initial values
static unsigned char* tempCorrected_INIT      = 0;
static unsigned char* sysPressCorrected_INIT  = 0;
static unsigned char* diasPressCorrected_INIT = 0;
static unsigned char* pulseRateCorrected_INIT = 0;

// Status initial value
static unsigned short batteryState_INIT = 200;

// Alarms initial values
static unsigned char bpOutOfRange_INIT    = 0;
static unsigned char tempOutOfRange_INIT  = 0;
static unsigned char pulseOutOfRange_INIT = 0;

// Warning initial values
static bool bpHigh_INIT   = false;
static bool tempHigh_INIT = false;
static bool pulseLow_INIT = false;

#endif /* INIT_H_ */
