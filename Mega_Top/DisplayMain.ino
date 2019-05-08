/*
 * Function name: Display
 * Function inputs: data structures for measurements, computations, display, warning and alarms, status, and scheduler, as well
 *                  as the rawstructs file, initialize file, and init file.
 * Function outputs: a display on a TFT display.
 * Function description: this uses the information gathered and properly uploads it onto the TFT display to be shown for
 *                       the user of the device.
 * Author: Haomin Yu
 */
// Compiler Linkage
extern "C" {
  #include "rawStructs.h"
  #include "initialize.h"
  #include "init.h"
}
// Global structures
MeasureDataStruct MeasureData;                                // Saves measurement data
DisplayDataStruct DisplayData;                                // Saves display data
ComputeDataStruct ComputeData;                                // Saves computational data
WarningAlarmDataStruct WarningAlarmData;                      // Saves warnign and alarm data
StatusStruct    Status;                                       // Saves status data
SchedulerStruct Scheduler;                                    // Saves scheduler
TFTKeypadDataStruct KeypadData;                               // Saves keypad data
CommunicationsDataStruct CommunicationsData;                  // Saves communications data
// Global indicator
extern bool tempRawChanged;
extern bool sysPressRawChanged;
extern bool diasPressRawChanged;
extern bool pulseRateRawChanged;

void userSetup() {                                            // Initializes the display to its initial display
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
             &Status, &Scheduler,
             &KeypadData, &CommunicationsData);
  // Baud rate for data transmittion
  Serial1.begin(9600);
}

void loop() {
   scheduler();                                                // Calls scheduler function to run the schedule
   touchScreen();
}
