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
	unsigned int    temperatureRawBuf[8];
	unsigned int    bloodPressRawBuf[16];
	unsigned int    pulseRateRawBuf[8];
  unsigned short* measurementSelection;
  
  unsigned short* nextTemperatureIndex;
  unsigned short* nextSysPressIndex;
  unsigned short* nextDiasPressIndex;
  unsigned short* nextPulseRateIndex;
};
typedef struct MeasureData MeasureDataStruct;

// Data structure for computation
struct ComputeData {
	unsigned int temperatureRawBuf[8];
  unsigned int bloodPressRawBuf[16];
  unsigned int pulseRateRawBuf[8];
	double tempCorrectedBuf[8];
	double bloodPressCorrectedBuf[16];
	double prCorrectedBuf[8];
  unsigned short* measurementSelection;

  unsigned short* nextTemperatureIndex;
  unsigned short* nextSysPressIndex;
  unsigned short* nextDiasPressIndex;
  unsigned short* nextPulseRateIndex;
};
typedef struct ComputeData ComputeDataStruct;

// Data structure for display
struct DisplayData {
	double tempCorrectedBuf[8];
  double bloodPressCorrectedBuf[16];
  double prCorrectedBuf[8];
	unsigned short* batteryState;

  unsigned short* nextTemperatureIndex;
  unsigned short* nextSysPressIndex;
  unsigned short* nextDiasPressIndex;
  unsigned short* nextPulseRateIndex;
};
typedef struct DisplayData DisplayDataStruct;

// Data structure for warning/alarm
struct WarningAlarmData {
	unsigned int    temperatureRawBuf[8];
  unsigned int    bloodPressRawBuf[16];
  unsigned int    pulseRateRawBuf[8];
	unsigned short* batteryState;
 
  unsigned char* bpOutOfRange;
  unsigned char* tempOutOfRange;
  unsigned char* pulseOutOfRange;
  unsigned char* batteryOutOfRange;

  bool* bpHigh;
  bool* tempHigh;
  bool* pulseLow;
  bool* batteryLow;

  unsigned short* nextTemperatureIndex;
  unsigned short* nextSysPressIndex;
  unsigned short* nextDiasPressIndex;
  unsigned short* nextPulseRateIndex;
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
  unsigned short* measurementSelection;
  unsigned short* alarmAcknowledge;
};
typedef struct TFTKeypadData TFTKeypadDataStruct;

// Data structure for communications data
struct CommunicationsData {
  double tempCorrectedBuf[8];
  double bloodPressCorrectedBuf[16];
  double prCorrectedBuf[8];
};
typedef struct CommunicationsData CommunicationsDataStruct;

#endif /* RAWSTRUCTS_H_ */
