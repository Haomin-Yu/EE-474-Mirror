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
// Global indicator
extern bool newTempComputed;
extern bool newBloodPressComputed;
extern bool newPulseRateComputed;
extern bool newRespirationComputed;

long pulseBlinkTime;
long tempBlinkTime;
long bpBlinkTime;
bool pulseBlink;
bool tempBlink;
bool bpBlink;
// Display function of the system
void display(void* Data) {
   DisplayDataStruct data = *((DisplayDataStruct*)Data);
   // Updates the measurements on the board if data is updated.

   if (millis() > (tempBlinkTime + 500)) {
          tempBlinkTime = millis();
          tempBlink = true;
    }
    if (millis() > (bpBlinkTime + 250)) {
          bpBlinkTime = millis();
          bpBlink = true;
    }
    if (millis() > (pulseBlinkTime + 1000)) {
          pulseBlinkTime = millis();
          pulseBlink = true;
    }
   if(newTempComputed || newBloodPressComputed || newPulseRateComputed || newRespirationComputed || pulseBlink || bpBlink || tempBlink) {
    updateMeasurements(data.tempCorrectedBuf[*data.currentTemperatureIndex], 
                             data.bloodPressCorrectedBuf[*data.currentSysPressIndex],
                             data.bloodPressCorrectedBuf[*data.currentDiasPressIndex],
                             data.prCorrectedBuf[*data.currentPulseRateIndex],
                             data.respirationCorrectedBuf[*data.currentRespirationIndex],
                             *data.batteryState);
   }
}
