/*
 * Function name: Auxiliary_Methods
 * Function inputs: warning data struct to tell if there are any warnings or alarms
 * Function outputs: contains many miscillanious methods mentioned below such as tft labels.
 * Function description: Creatues many miscillanious methods to be used throughout
 *                       the rest of out programs so it can be cleaner, easier to understand, and easier to call.
 * Author: Haomin Yu
 *
 */


// Writes 'content' in the given 'color' at position (x, y)
void TFT_Write(int Color, int x, int y, String content) {
// Sets the color of the text
  tft.setTextColor(Color);
// Sets the position of the text
  tft.setCursor(x, y);
// Prints the text
  tft.print(content);
}
// Sets up labels
void labelsInit() {
  TFT_Write(GREEN, 10, 65,  "Body.Temp  ->        C");
  TFT_Write(GREEN, 10, 90,  "Sys.Press  ->        mmHg"); 
  TFT_Write(GREEN, 10, 115, "Dias.Press ->        mmHg"); 
  TFT_Write(GREEN, 10, 140, "Pulse Rate ->        BPM"); 
  TFT_Write(GREEN, 10, 165, "Battery    ->"); 
}
// Updates the measurement values(Erases the previous value)
extern WarningAlarmDataStruct WarningAlarmData;
void updateMeasurements(double tempCorrected, 
                        double systolicPressCorrected,
                        double diastolicPressCorrected,
                        double pulseRateCorrected,
                        unsigned short batteryState) {
// Erasing the measurements
  tft.fillRect(170, 65, 80, 115, BLACK);
// Updating the measurements
  int tempColor = (*WarningAlarmData.tempHigh   || *WarningAlarmData.tempOutOfRange)?
                    RED: GREEN;
  int bpColor   = (*WarningAlarmData.bpHigh     || *WarningAlarmData.bpOutOfRange)?
                    RED: GREEN;
  int prColor   = (*WarningAlarmData.pulseLow   || *WarningAlarmData.pulseOutOfRange)?
                    RED: GREEN;
  int battColor = (*WarningAlarmData.batteryLow || *WarningAlarmData.batteryOutOfRange)?
                    RED: GREEN;
  TFT_Write(tempColor, 175, 65,  (String)tempCorrected);
  TFT_Write(bpColor  , 175, 90,  (String)systolicPressCorrected); 
  TFT_Write(bpColor  , 175, 115, (String)diastolicPressCorrected); 
  TFT_Write(prColor  , 175, 140, (String)pulseRateCorrected); 
  TFT_Write(battColor, 175, 165, (String)batteryState); 
}
// Gets an unsigned int from the Uno
unsigned int getSerialUInt() {
  while(Serial1.available() == 0) {}
  return Serial1.read();
}
// Calls on the Uno to get the temperature
unsigned int getSerialTemp() {
  Serial1.write(0x00);
  return getSerialUInt();
}
// Calls on the Uno to get the systolic pressure
unsigned int getSysPress() {
  Serial1.write(0x01);
  return getSerialUInt();
}
// Calls on the Uno to get the diastolic pressure
unsigned int getDiasPress() {
  Serial1.write(0x02);
  return getSerialUInt();
}
// Calls on the Uno to get the pulse rate
unsigned int getPulseRate() {
  Serial1.write(0x03);
  return getSerialUInt();
}
