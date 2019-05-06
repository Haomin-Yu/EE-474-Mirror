/*
 * Function name: initialize
 * Function inputs: inputs the raw data structures as well as the initial values
 * Function outputs: outputs the structures with the intial values set for their initial value.
 * Function description: Sets the values of all the structs to the initial values they are
 *                       meant to be set too.
 * Author: Haomin Yu
 *
 */
#include "init.h"
#include "rawStructs.h"

// Initializing global variables
void initialize(MeasureDataStruct* MeasureData,
                DisplayDataStruct* DisplayData,
                ComputeDataStruct* ComputeData,
                WarningAlarmDataStruct* WarningAlarmData,
                StatusStruct*    Status,
                SchedulerStruct* Scheduler,
                TFTKeypadDataStruct* KeypadData, 
                CommunicationsDataStruct* CommunicationsData);
void initialize(MeasureDataStruct* MeasureData,
                DisplayDataStruct* DisplayData,
                ComputeDataStruct* ComputeData,
                WarningAlarmDataStruct* WarningAlarmData,
                StatusStruct*    Status,
                SchedulerStruct* Scheduler,
                TFTKeypadDataStruct* KeypadData, 
                CommunicationsDataStruct* CommunicationsData) {
	// Initializing measurement variables
  *MeasureData = (MeasureDataStruct) {
     temperatureRawBuf_INIT,
     bloodPressRawBuf_INIT,
     pulseRateRawBuf_INIT,
     &measurementSelection_INIT,

     &nextTemperatureIndex_INIT,
     &nextSysPressIndex_INIT,
     &nextDiasPressIndex_INIT,
     &nextPulseRateIndex_INIT
  };
	// Initializing display variables
  *DisplayData = (DisplayDataStruct) {
     tempCorrectedBuf_INIT,
     bloodPressCorrectedBuf_INIT,
     pulseRateCorrectedBuf_INIT,
     &batteryState_INIT,

     &nextTemperatureIndex_INIT,
     &nextSysPressIndex_INIT,
     &nextDiasPressIndex_INIT,
     &nextPulseRateIndex_INIT
  };
	// Initializing compute variables
  *ComputeData = (ComputeDataStruct) {
     temperatureRawBuf_INIT,
     bloodPressRawBuf_INIT,
     pulseRateRawBuf_INIT,
     tempCorrectedBuf_INIT,
     bloodPressCorrectedBuf_INIT,
     pulseRateCorrectedBuf_INIT,
     &measurementSelection_INIT,

     &nextTemperatureIndex_INIT,
     &nextSysPressIndex_INIT,
     &nextDiasPressIndex_INIT,
     &nextPulseRateIndex_INIT
  };
	// Initializing warning/alarm variables
  *WarningAlarmData = (WarningAlarmDataStruct) {
     temperatureRawBuf_INIT,
     bloodPressRawBuf_INIT,
     pulseRateRawBuf_INIT,
     &batteryState_INIT,
     
     &bpOutOfRange_INIT,
     &tempOutOfRange_INIT,
     &pulseOutOfRange_INIT,
     &batteryOutOfRange_INIT,
     
     &bpHigh_INIT,
     &tempHigh_INIT,
     &pulseLow_INIT,
     &batteryLow,

     &nextTemperatureIndex_INIT,
     &nextSysPressIndex_INIT,
     &nextDiasPressIndex_INIT,
     &nextPulseRateIndex_INIT
  };
	// Initializing status variables
  *Status = (StatusStruct) {
     &batteryState_INIT
  };
  // Initializing keypad data
  *KeypadData = (TFTKeypadDataStruct) {
     &measurementSelection_INIT,
     &alarmAcknowledgement_INIT
  };
  // Initializing communications data
  *CommunicationsData = (CommunicationsDataStruct) {
     tempCorrectedBuf_INIT,
     bloodPressCorrectedBuf_INIT,
     pulseRateCorrectedBuf_INIT
  };
}
