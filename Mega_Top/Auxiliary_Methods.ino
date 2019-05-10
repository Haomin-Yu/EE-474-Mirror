/*
 * Function name: Auxiliary_Methods
 * Function inputs: warning data struct to tell if there are any warnings or alarms
 * Function outputs: contains many miscillanious methods mentioned below such as tft labels.
 * Function description: Creatues many miscillanious methods to be used throughout
 *                       the rest of out programs so it can be cleaner, easier to understand, and easier to call.
 * Author: Haomin Yu
 *
 */
extern "C" {
  #include "rawStructs.h";
}
extern MeasureDataStruct MeasureData;
extern WarningAlarmDataStruct WarningAlarmData;
extern bool newTempComputed;
extern bool newSysPressComputed;
extern bool newDiasPressComputed;
extern bool newPulseRateComputed;
#define MINPRESSURE  10
#define MAXPRESSURE  1000

bool alarmButton = false;

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
  TFT_Write(GREEN, 10, 23,  "Body.Temp  ->        C");
  TFT_Write(GREEN, 10, 48,  "Sys.Press  ->        mmHg"); 
  TFT_Write(GREEN, 10, 73,  "Dias.Press ->        mmHg"); 
  TFT_Write(GREEN, 10, 98, "Pulse Rate ->        BPM"); 
  TFT_Write(GREEN, 10, 123, "Battery    ->");
  tft.fillRect(10, 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((12 + BUTTONWIDTH), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((14 + BUTTONWIDTH * 2), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((16 + BUTTONWIDTH * 3), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((54 + BUTTONWIDTH), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  TFT_Write(RED, 12, 175, " Temp.");
  TFT_Write(RED, (14 + BUTTONWIDTH), 175, " Sys.");
  TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " Dias.");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
  TFT_Write(RED, (60 + BUTTONWIDTH), 217,"Alarm");
}
// Updates the measurement values(Erases the previous value)
void updateMeasurements(double tempCorrected, 
                        double systolicPressCorrected,
                        double diastolicPressCorrected,
                        double pulseRateCorrected,
                        unsigned short batteryState) {
  // Updating the measurements
  int tempColor = (*WarningAlarmData.tempHigh   || *WarningAlarmData.tempOutOfRange)?
                    RED: GREEN;
  int bpColor   = (*WarningAlarmData.bpHigh     || *WarningAlarmData.bpOutOfRange)?
                    RED: GREEN;
  int prColor   = (*WarningAlarmData.pulseLow   || *WarningAlarmData.pulseOutOfRange)?
                    RED: GREEN;
  int battColor = (*WarningAlarmData.batteryLow || *WarningAlarmData.batteryOutOfRange)?
                    RED: GREEN;
  bool newBatteryUpdate = newTempComputed || newSysPressComputed || newDiasPressComputed ||
                          newPulseRateComputed;
  if(newTempComputed) {
     tft.fillRect(175, 23, 80, 24, BLACK);
     TFT_Write(tempColor, 175, 23,  (String)tempCorrected);
     newTempComputed = false;
  }
  if(newSysPressComputed) {
     tft.fillRect(175, 48, 80, 24, BLACK);
     TFT_Write(bpColor  , 175, 48,  (String)systolicPressCorrected); 
     newSysPressComputed = false;
  }
  if(newDiasPressComputed) {
     tft.fillRect(175, 73, 80, 24, BLACK);
     TFT_Write(bpColor  , 175, 73,  (String)diastolicPressCorrected); 
     newDiasPressComputed = false;
  }
  if(newPulseRateComputed) {
     tft.fillRect(175, 98, 80, 24, BLACK);
     TFT_Write(prColor  , 175, 98,  (String)(int)pulseRateCorrected); 
     newPulseRateComputed = false;
  }
  if(newBatteryUpdate) {
     tft.fillRect(175, 123, 80, 24, BLACK);
     TFT_Write(battColor, 175, 123, (String)batteryState); 
  }
}

void touchScreen() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (!alarmAcknowledged && alarmCheck) {
    tft.fillRect(0, 0, 320, 20, RED);
    TFT_Write(WHITE, 5, 3, "ALARM ACTIVE  ALARM ACTIVE");
    alarmCheck = false;
  }

  p.x = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);
  //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = (tft.height()-map(p.y, TS_MINX, TS_MAXX, tft.width(), 0));
  //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
     if((p.x < (BUTTONHEIGHT + 160)) && (p.x > 160) && alarmAcknowledged) {
        if(((tft.height()-p.y) < (BUTTONWIDTH + 10)) && ((tft.height()-p.y) > 10)) {
           tempCheck = true;
           *MeasureData.measurementSelection = 1; 
           tft.fillRect(10, 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, 12, 175, "Temp.");
        }
        else if(((tft.height()-p.y) < (12 + BUTTONWIDTH * 2)) && ((tft.height()-p.y) > (12 + BUTTONWIDTH))) {
           sysCheck = true;
           *MeasureData.measurementSelection = 2; 
           tft.fillRect((12 + BUTTONWIDTH), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (14 + BUTTONWIDTH), 175, "Sys.");
        } 
        else if(((tft.height()-p.y) < (14 + BUTTONWIDTH * 3)) && ((tft.height()-p.y) > (14 + BUTTONWIDTH * 2))) {
           diasCheck = true;
           *MeasureData.measurementSelection = 3; 
           tft.fillRect((14 + BUTTONWIDTH * 2), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, "Dias.");
        } 
        else if(((tft.height()-p.y) < (16 + BUTTONWIDTH * 4)) && ((tft.height()-p.y) > (16 + BUTTONWIDTH * 3))) {
           pulseCheck = true;
           *MeasureData.measurementSelection = 4; 
           tft.fillRect((16 + BUTTONWIDTH * 3), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
        }
     }
     else if((p.x < (BUTTONHEIGHT + 202)) && (p.x > 202)) {
      if(((tft.height()-p.y) < ((BUTTONWIDTH *2) + 54)) && ((tft.height()-p.y) > (54 + BUTTONWIDTH))) {
           alarmAcknowledged = true;
           alarmButton = true;
           tft.fillRect(0, 0, 320, 20, BLACK);
           tft.fillRect((54 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
        }
     } 
  } else if (alarmButton){
      alarmButton = false;
      tft.fillRect((54 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
      TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
  }
}

// Gets an unsigned int from the Uno
unsigned int getSerialUInt() {
  while(Serial1.available() == 0) {}
  return Serial1.read();
}
// Calls on the Uno to get the temperature
unsigned int getSerialTemp() {
  if(tempCheck) {
    Serial1.write(0x00);
    tempCheck = false;
    return getSerialUInt();
  }
}
// Calls on the Uno to get the systolic pressure
unsigned int getSysPress() {
  if(sysCheck){
    Serial1.write(0x01);
    sysCheck = false;
    return getSerialUInt();
  }
}
// Calls on the Uno to get the diastolic pressure
unsigned int getDiasPress() {
  if(diasCheck) {
    Serial1.write(0x02);
    diasCheck = false;
    return getSerialUInt();
  }
}
// Calls on the Uno to get the pulse rate
unsigned int getPulseRate() {
  if(pulseCheck) {
    Serial1.write(0x03);
    pulseCheck = false;
    return getSerialUInt();
  }
}
