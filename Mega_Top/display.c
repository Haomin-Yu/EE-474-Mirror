/*
 * Function name: display
 * Function inputs: inputs the rawstructsas well as the string.h fle
 * Function outputs: outputs the data to be displayed
 * Function description: it is used to set the actual calcultated values to be displayed
 * Author: Haomin Yu
 */
#include "rawStructs.h"
#include <string.h>

// Function Prototypes
void display(void* Data);
// Global parameter
bool tempChanged      = true;
bool sysPressChanged  = true;
bool diasPressChanged = true;
bool pulseRateChanged = true;

// Display function of the system
void display(void* Data) {
   DisplayDataStruct data = *((DisplayDataStruct*)Data);
   data.tempCorrectedBuf[*data.currentTemperatureIndex]     = data.tempCorrectedBuf[*data.currentTemperatureIndex];
   data.bloodPressCorrectedBuf[*data.currentSysPressIndex]  = data.bloodPressCorrectedBuf[*data.currentSysPressIndex];
   data.bloodPressCorrectedBuf[*data.currentDiasPressIndex] = data.bloodPressCorrectedBuf[*data.currentDiasPressIndex];
   data.prCorrectedBuf[*data.currentDiasPressIndex]         = data.prCorrectedBuf[*data.currentDiasPressIndex];
}
