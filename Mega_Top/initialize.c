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
                SchedulerStruct* Scheduler);
void initialize(MeasureDataStruct* MeasureData,
                DisplayDataStruct* DisplayData,
                ComputeDataStruct* ComputeData,
                WarningAlarmDataStruct* WarningAlarmData,
                StatusStruct*    Status,
                SchedulerStruct* Scheduler) {
	// Initializing measurement variables
  *MeasureData = (MeasureDataStruct) {
     &temperatureRaw_INIT,
     &sysPressRaw_INIT,
     &diasPressRaw_INIT,
     &pulseRateRaw_INIT
  };
	// Initializing display variables
  *DisplayData = (DisplayDataStruct) {
     &tempCorrected_INIT,
     &sysPressCorrected_INIT,
     &diasPressCorrected_INIT,
     &pulseRateCorrected_INIT,
     &batteryState_INIT
  };
	// Initializing compute variables
  *ComputeData = (ComputeDataStruct) {
     &temperatureRaw_INIT,
     &sysPressRaw_INIT,
     &diasPressRaw_INIT,
     &pulseRateRaw_INIT,
     &tempCorrected_INIT,
     &sysPressCorrected_INIT,
     &diasPressCorrected_INIT,
     &pulseRateCorrected_INIT
  };
	// Initializing warning/alarm variables
  *WarningAlarmData = (WarningAlarmDataStruct) {
     &temperatureRaw_INIT,
     &sysPressRaw_INIT,
     &diasPressRaw_INIT,
     &pulseRateRaw_INIT,
     &batteryState_INIT,
     &bpOutOfRange_INIT,
     &tempOutOfRange_INIT,
     &pulseOutOfRange_INIT,
     &batteryOutOfRange_INIT,
     &bpHigh_INIT,
     &tempHigh_INIT,
     &pulseLow_INIT,
     &batteryLow
  };
	// Initializing status variables
  *Status = (StatusStruct) {
     &batteryState_INIT
  };
}
