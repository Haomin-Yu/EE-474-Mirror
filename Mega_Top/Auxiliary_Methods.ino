/*
 * Function name: Auxiliary_Methods
 * Function inputs: warning data struct to tell if there are any warnings or alarms
 * Function outputs: contains many miscillanious methods mentioned below such as tft labels.
 * Function description: Creatues many miscillanious methods to be used throughout
 *                       the rest of out programs so it can be cleaner, easier to understand, and easier to call.
 * Author: Haomin Yu
 *
 */

#define BUTTONWIDTH 80
#define BUTTONHEIGHT 40
#define MINPRESSURE 10
#define MAXPRESSURE 1000

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
  TFT_Write(GREEN, 10, 30,  "Body.Temp  ->        C");
  TFT_Write(GREEN, 10, 65,  "Sys.Press  ->        mmHg"); 
  TFT_Write(GREEN, 10, 90,  "Dias.Press ->        mmHg"); 
  TFT_Write(GREEN, 10, 115, "Pulse Rate ->        BPM"); 
  TFT_Write(GREEN, 10, 140, "Battery    ->");
  tft.fillRect(10, 165, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((12 + BUTTONWIDTH), 165, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((14 + BUTTONWIDTH * 2), 165, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((16 + BUTTONWIDTH * 3), 165, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  TFT_Write(RED, 12, 180, "Temp.");
  TFT_Write(RED, (14 + BUTTONWIDTH), 180, "Sys.");
  TFT_Write(RED, (16 + BUTTONWIDTH * 2), 180, "Dias.");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 180, "Pulse");
}
// Updates the measurement values(Erases the previous value)
extern WarningAlarmDataStruct WarningAlarmData;
void updateMeasurements(double tempCorrected, 
                        double systolicPressCorrected,
                        double diastolicPressCorrected,
                        double pulseRateCorrected,
                        unsigned short batteryState) {
// Erasing the measurements
  tft.fillRect(170, 30, 80, 125, BLACK);
// Updating the measurements
  int tempColor = (*WarningAlarmData.tempHigh   || *WarningAlarmData.tempOutOfRange)?
                    RED: GREEN;
  int bpColor   = (*WarningAlarmData.bpHigh     || *WarningAlarmData.bpOutOfRange)?
                    RED: GREEN;
  int prColor   = (*WarningAlarmData.pulseLow   || *WarningAlarmData.pulseOutOfRange)?
                    RED: GREEN;
  int battColor = (*WarningAlarmData.batteryLow || *WarningAlarmData.batteryOutOfRange)?
                    RED: GREEN;
  TFT_Write(tempColor, 175, 30,  (String)tempCorrected);
  TFT_Write(bpColor  , 175, 65,  (String)systolicPressCorrected); 
  TFT_Write(bpColor  , 175, 90, (String)diastolicPressCorrected); 
  TFT_Write(prColor  , 175, 115, (String)pulseRateCorrected); 
  TFT_Write(battColor, 175, 140, (String)batteryState); 
}

void touchScreen() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  p.x = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);
  //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = (tft.height()-map(p.y, TS_MINX, TS_MAXX, tft.width(), 0));
  //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
     if((p.x < (BUTTONHEIGHT + 165)) && (p.x > 165)) {
        if(((tft.height()-p.y) < (BUTTONWIDTH + 10)) && ((tft.height()-p.y) > 10)) {
           tempCheck = true;
           tft.fillScreen(GREEN);
        }
        else if(((tft.height()-p.y) < (12 + BUTTONWIDTH * 2)) && ((tft.height()-p.y) > (12 + BUTTONWIDTH))) {
           sysCheck = true;
           tft.fillScreen(RED);
        } 
        else if(((tft.height()-p.y) < (14 + BUTTONWIDTH * 3)) && ((tft.height()-p.y) > (14 + BUTTONWIDTH * 2))) {
           diasCheck = true;
           tft.fillScreen(BLUE);
        } 
        else if(((tft.height()-p.y) < (16 + BUTTONWIDTH * 4)) && ((tft.height()-p.y) > (16 + BUTTONWIDTH * 3))) {
           pulseCheck = true;
           tft.fillScreen(CYAN);
        }
     }
  }
  if(tempCheck) {
    tft.fillRect(10, 165, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
    TFT_Write(RED, 12, 180, "Temp.");
  } else {
    tft.fillRect(10, 165, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
    TFT_Write(RED, 12, 180, "Temp.");
  }
  if(sysCheck) {
    tft.fillRect((12 + BUTTONWIDTH), 165, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
    TFT_Write(RED, (14 + BUTTONWIDTH), 180, "Sys.");
  } else {
    tft.fillRect((12 + BUTTONWIDTH), 165, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
    TFT_Write(RED, (14 + BUTTONWIDTH), 180, "Sys.");
  }
  if(diasCheck) {
    tft.fillRect((14 + BUTTONWIDTH * 2), 165, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
    TFT_Write(RED, (16 + BUTTONWIDTH * 2), 180, "Dias.");
  } else {
    tft.fillRect((14 + BUTTONWIDTH * 2), 165, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
    TFT_Write(RED, (16 + BUTTONWIDTH * 2), 180, "Dias.");
  }
  if(pulseCheck) {
    tft.fillRect((16 + BUTTONWIDTH * 3), 165, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
    TFT_Write(RED, (18 + BUTTONWIDTH * 3), 180, "Pulse");
  } else {
    tft.fillRect((16 + BUTTONWIDTH * 3), 165, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
    TFT_Write(RED, (18 + BUTTONWIDTH * 3), 180, "Pulse");
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
