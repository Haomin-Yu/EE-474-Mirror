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
#include <stdbool.h>

#ifndef RAWSTRUCTS_H_
#define RAWSTRUCTS_H_

// Represents one executable task, which holds a
// pointer to a function and pointer to related data
struct Task {
	void (*myTask)(void*);
	void* taskDataPtr;
  struct Task* next;
  struct Task* prev;
};
typedef struct Task TCB;

// Data structure for measurement
struct MeasureData {
	unsigned int* temperatureRawBuf;
	unsigned int* bloodPressRawBuf;
	unsigned int* pulseRateRawBuf;
  unsigned int* respirationRateRawBuf;
  unsigned short* measurementSelection;
  
  unsigned short* currentTemperatureIndex;
  unsigned short* currentSysPressIndex;
  unsigned short* currentDiasPressIndex;
  unsigned short* currentPulseRateIndex;
  unsigned short* currentRespirationIndex;
};
typedef struct MeasureData MeasureDataStruct;

// Data structure for computation
struct ComputeData {
	unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned int* respirationRateRawBuf;
	double* tempCorrectedBuf;
	double* bloodPressCorrectedBuf;
	double* prCorrectedBuf;
  double* respirationRateCorrectedBuf;
  unsigned short* measurementSelection;

  unsigned short* currentTemperatureIndex;
  unsigned short* currentSysPressIndex;
  unsigned short* currentDiasPressIndex;
  unsigned short* currentPulseRateIndex;
  unsigned short* currentRespirationIndex;
};
typedef struct ComputeData ComputeDataStruct;

// Data structure for display
struct DisplayData {
  unsigned short* mode;
	double* tempCorrectedBuf;
  double* bloodPressCorrectedBuf;
  double* prCorrectedBuf;
  double* respirationCorrectedBuf;
	unsigned short* batteryState;

  unsigned short* currentTemperatureIndex;
  unsigned short* currentSysPressIndex;
  unsigned short* currentDiasPressIndex;
  unsigned short* currentPulseRateIndex;
  unsigned short* currentRespirationIndex;
};
typedef struct DisplayData DisplayDataStruct;

// Data structure for warning/alarm
struct WarningAlarmData {
	unsigned int* temperatureRawBuf;
  unsigned int* bloodPressRawBuf;
  unsigned int* pulseRateRawBuf;
  unsigned int* respirationRateRawBuf;
	unsigned short* batteryState;
 
  unsigned char* bpOutOfRange;
  unsigned char* tempOutOfRange;
  unsigned char* pulseOutOfRange;
  unsigned char* batteryOutOfRange;
  unsigned char* respOutOfRange;

  bool *respHigh;
  bool* bpHigh;
  bool* tempHigh;
  bool* pulseLow;
  bool* batteryLow;

  unsigned short* currentTemperatureIndex;
  unsigned short* currentSysPressIndex;
  unsigned short* currentDiasPressIndex;
  unsigned short* currentPulseRateIndex;
  unsigned short* currentRespirationRateIndex;
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

// Data structure for TFT-keypad data
struct TFTKeypadData {
  unsigned short* localMeasurementSelection;
  unsigned short* alarmAcknowledge;
  unsigned short* remoteMeasurementSelection;
};
typedef struct TFTKeypadData TFTKeypadDataStruct;

// Data structure for communications data
struct CommunicationsData {
  double* tempCorrectedBuf;
  double* bloodPressCorrectedBuf;
  double* prCorrectedBuf;
  double* respirationRateCorrectedBuf;

  unsigned short* currentTemperatureIndex;
  unsigned short* currentSysPressIndex;
  unsigned short* currentDiasPressIndex;
  unsigned short* currentPulseRateIndex;
  unsigned short* currentRespirationRateIndex;
};
typedef struct CommunicationsData CommunicationsDataStruct;

#endif /* RAWSTRUCTS_H_ */
