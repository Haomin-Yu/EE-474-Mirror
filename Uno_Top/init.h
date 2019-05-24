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
static unsigned int temperatureRaw_INIT   = 75;
static unsigned int bloodPressureRaw_INIT = 80;
static unsigned int pulseRateRaw_INIT     = 0;
static unsigned int respirationRateRaw_INIT = 0;

// Status initial value
static unsigned short batteryState_INIT = 200;

#endif /* INIT_H_ */
