/*
 * Function name: Display
 * Function inputs: data structures for measurements, computations, display, warning and alarms, status, and scheduler, as well
 *                  as the rawstructs file, initialize file, and init file.
 * Function outputs: a display on a TFT display.
 * Function description: this uses the information gathered and properly uploads it onto the TFT display to be shown for
 *                       the user of the device.
 * Author: Haomin Yu
 */
extern "C" {                                                  //calls in created files to be used.
  #include "rawStructs.h"
  #include "initialize.h"
  #include "init.h"
}
// Global structures
MeasureDataStruct MeasureData;                                //saves measurement data
DisplayDataStruct DisplayData;                                //saves display data
ComputeDataStruct ComputeData;                                //saves computational data
WarningAlarmDataStruct WarningAlarmData;                      //saves warnign and alarm data
StatusStruct    Status;                                       //saves status data
SchedulerStruct Scheduler;                                    //saves scheduler
// Global indicator
extern bool measurementChanged;

void userSetup() {                                            //initializes the display to its initial display.
  // Makes the whole screen black
  tft.fillScreen(BLACK);
  // Sets texts to font 3
  tft.setTextSize(2);
  // Makes the text display horizontally
  tft.setRotation(HORIZONTAL);
  // Sets up the labels
  labelsInit();
  // Initializes the values
  initialize(&MeasureData, &DisplayData,
             &ComputeData, &WarningAlarmData,
             &Status, &Scheduler);
  // Baud rate for data transmittion
  Serial1.begin(9600);
}

void loop() {
   scheduler();                                                 //calls scheduler function to runthe schedule
   if(measurementChanged) {                                     //updates the measurements on the board if data is updated.
     updateMeasurements(*DisplayData.tempCorrected, 
                        *DisplayData.sysPressCorrected,
                        *DisplayData.diasCorrected,
                        *DisplayData.prCorrected,
                        *DisplayData.batteryState);
   }
}
