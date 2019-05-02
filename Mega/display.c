/*
 * Function name: display
 * Function inputs: inputs the rawstructsas well as the string.h fle
 * Function outputs: outputs the data to be displayed
 * Function description: it is used to set the actual calcultated values to be displayed.
 * Author: Haomin Yu
 */
#include "rawStructs.h"
#include <string.h>

// Function Prototypes
void display(void* Data);
// Global parameter
bool measurementChanged = true;
bool enableDisplay      = true;

// Display of the system
void display(void* Data) {
  if(enableDisplay) {
    DisplayDataStruct data = *((DisplayDataStruct*)Data);
    *data.tempCorrected = *data.tempCorrected;
    *data.sysPressCorrected = *data.sysPressCorrected;
    *data.diasCorrected = *data.diasCorrected;
    *data.prCorrected = *data.prCorrected;
  }
  measurementChanged = enableDisplay;
}
