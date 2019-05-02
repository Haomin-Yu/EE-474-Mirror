/*
 * Function name: rawStructs
 * Function inputs: inputs the stdbool and init files
 * Function outputs: creates structures for all the main portions the code will be using
 * Function description: The function is simply to define what a task is and create all the
 *                       structures that are used in creating this program such as measure
 *                       or compute.
 * Author: Haomin Yu
 */
#pragma once
#include "init.h"
#include <stdbool.h>

#ifndef RAWSTRUCTS_H_
#define RAWSTRUCTS_H_

// Represents one executable task, which holds a
// pointer to a function and pointer to related data
struct Task {
	void (*myTask)(void*);
	void* taskDataPtr;
};
typedef struct Task TCB;

// Data structure for measurement
struct MeasureData {
	unsigned int* temperatureRaw;
	unsigned int* systolicPressRaw;
	unsigned int* diastolicPressRaw;
	unsigned int* pulseRateRaw;
};
typedef struct MeasureData MeasureDataStruct;

// Data structure for computation
struct ComputeData {
	unsigned int* temperatureRaw;
	unsigned int* systolicPressRaw;
	unsigned int* diastolicPressRaw;
	unsigned int* pulseRateRaw;
	double* tempCorrected;
	double* sysPressCorrected;
	double* diasPressCorrected;
	double* prCorrected;
};
typedef struct ComputeData ComputeDataStruct;

// Data structure for display
struct DisplayData {
	double* tempCorrected;
	double* sysPressCorrected;
	double* diasCorrected;
	double* prCorrected;
	unsigned short* batteryState;
};
typedef struct DisplayData DisplayDataStruct;

// Data structure for warning/alarm
struct WarningAlarmData {
	unsigned int* temperatureRaw;
	unsigned int* systolicPressRaw;
  unsigned int* diastolicPressRaw;
	unsigned int* pulseRateRaw;
	unsigned short* batteryState;
 
  unsigned char* bpOutOfRange;
  unsigned char* tempOutOfRange;
  unsigned char* pulseOutOfRange;
  unsigned char* batteryOutOfRange;

  bool* bpHigh;
  bool* tempHigh;
  bool* pulseLow;
  bool* batteryLow;
};
typedef struct WarningAlarmData WarningAlarmDataStruct;

// Data structure for status
struct Status {
	unsigned short* batteryState;
};
typedef struct Status StatusStruct;

// Data structure for the scheduler
struct Scheduler {
	// Empty
};
typedef struct Scheduler SchedulerStruct;

#endif /* RAWSTRUCTS_H_ */
