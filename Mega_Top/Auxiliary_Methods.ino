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
extern TFTKeypadDataStruct KeypadData;
extern bool newTempComputed;
extern bool newSysPressComputed;
extern bool newDiasPressComputed;
extern bool newPulseRateComputed;
extern const unsigned long BUTTON_TIME;
extern unsigned long previousTime;
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
  tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((16 + BUTTONWIDTH * 3), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  TFT_Write(RED, 12, 175, " Temp.");
  TFT_Write(RED, (14 + BUTTONWIDTH), 175, " Sys.");
  TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " Dias.");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
  TFT_Write(RED, (60 + BUTTONWIDTH), 217,"Alarm");
  TFT_Write(RED, 12, 217,"Blank");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 217,"Blank");
}
// Updates the measurement values(Erases the previous value)
void updateMeasurements(double tempCorrected, 
                        double systolicPressCorrected,
                        double diastolicPressCorrected,
                        double pulseRateCorrected,
                        unsigned short batteryState) {
  // Updating the measurements
  bool newBatteryUpdate = newTempComputed || newSysPressComputed || newDiasPressComputed ||
                          newPulseRateComputed;
  if(newTempComputed || alarmCheck) {
    int tempColor;
    if(*WarningAlarmData.tempOutOfRange) {
      tempColor = YELLOW;
    } else {
      tempColor = GREEN;
    }
     tft.fillRect(175, 23, 80, 24, BLACK);
     TFT_Write(tempColor, 175, 23,  (String)tempCorrected);
     newTempComputed = false;
  }
  if(newSysPressComputed || alarmCheck) {
    int bpColor;
    if(*WarningAlarmData.bpOutOfRange && (systolicPressCorrected > 156) && (annonciationCounter > 4)) {
      bpColor = RED;
    } else if (*WarningAlarmData.bpOutOfRange) {
      bpColor = YELLOW;
    } else {
      bpColor = GREEN;
    }
     tft.fillRect(175, 48, 80, 24, BLACK);
     TFT_Write(bpColor  , 175, 48,  (String)systolicPressCorrected); 
     newSysPressComputed = false;
  }
  if(newDiasPressComputed || alarmCheck) {
    int bpColor;
    if(*WarningAlarmData.bpOutOfRange) {
      bpColor = YELLOW;
    } else {
      bpColor = GREEN;
    }
     tft.fillRect(175, 73, 80, 24, BLACK);
     TFT_Write(bpColor  , 175, 73,  (String)diastolicPressCorrected); 
     newDiasPressComputed = false;
  }
  if(newPulseRateComputed || alarmCheck) {
     int pulseColor;
     if(*WarningAlarmData.pulseOutOfRange) {
       pulseColor = YELLOW;
     } else {
       pulseColor = GREEN;
     }
     tft.fillRect(175, 98, 80, 24, BLACK);
     TFT_Write(pulseColor  , 175, 98,  (String)(int)pulseRateCorrected); 
     newPulseRateComputed = false;
  }
  if(newBatteryUpdate) {
     int battColor;
     if(*WarningAlarmData.batteryOutOfRange && (annonciationCounter > 4)) {
       battColor = RED;
     } else if (*WarningAlarmData.batteryOutOfRange) {
       battColor = YELLOW;
     } else {
       battColor = GREEN;
     }
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

  if (!(*KeypadData.alarmAcknowledge == 0) && alarmCheck) {
    if (annonciationCounter > 4) {
      tft.fillRect(0, 0, 320, 20, RED);
      TFT_Write(WHITE, 5, 3, "ALARM ACTIVE  ALARM ACTIVE");
      alarmCheck = false;
    }
  }

  p.x = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);
  //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = (tft.height()-map(p.y, TS_MINX, TS_MAXX, tft.width(), 0));
  //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
     if((p.x < (BUTTONHEIGHT + 160)) && (p.x > 160) && (*KeypadData.alarmAcknowledge == 0)) {
        if(((tft.height()-p.y) < (BUTTONWIDTH + 10)) && ((tft.height()-p.y) > 10)) {
           tempCheck = true;
           *MeasureData.measurementSelection = 1; 
           *KeypadData.measurementSelection = 1; 
           tft.fillRect(10, 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, 12, 175, " Temp.");
        }
        else if(((tft.height()-p.y) < (12 + BUTTONWIDTH * 2)) && ((tft.height()-p.y) > (12 + BUTTONWIDTH))) {
           sysCheck = true;
           *MeasureData.measurementSelection = 2;
           *KeypadData.measurementSelection = 2; 
           tft.fillRect((12 + BUTTONWIDTH), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (14 + BUTTONWIDTH), 175, " Sys.");
        } 
        else if(((tft.height()-p.y) < (14 + BUTTONWIDTH * 3)) && ((tft.height()-p.y) > (14 + BUTTONWIDTH * 2))) {
           diasCheck = true;
           *MeasureData.measurementSelection = 3; 
           *KeypadData.measurementSelection = 3;
           tft.fillRect((14 + BUTTONWIDTH * 2), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " Dias.");
        } 
        else if(((tft.height()-p.y) < (16 + BUTTONWIDTH * 4)) && ((tft.height()-p.y) > (16 + BUTTONWIDTH * 3))) {
           pulseCheck = true;
           *MeasureData.measurementSelection = 4; 
           *KeypadData.measurementSelection = 4;
           tft.fillRect((16 + BUTTONWIDTH * 3), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
        }
     }
     else if((p.x < (BUTTONHEIGHT + 202)) && (p.x > 202)) {
      if(((tft.height()-p.y) < ((BUTTONWIDTH *2) + 54)) && ((tft.height()-p.y) > (54 + BUTTONWIDTH))) {
           
            annonciationCounter = 0;
           
           *KeypadData.alarmAcknowledge = 0;
           alarmButton = true;
           tft.fillRect((54 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, BLUE);
           TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
        } else if (((tft.height()-p.y) < ((BUTTONWIDTH) + 10)) && ((tft.height()-p.y) > 10)) {
          tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLUE);
          TFT_Write(RED, 12, 217,"Blank");
        } else if (((tft.height()-p.y) < ((16 + BUTTONWIDTH * 4))) && ((tft.height()-p.y) > 16)) {
          tft.fillRect((16 + BUTTONWIDTH * 3), 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLUE);
          TFT_Write(RED, (18 + BUTTONWIDTH * 3), 217,"Blank");
        }
     } 
  } else if (alarmButton && ((millis() - previousTime) > BUTTON_TIME)){
      previousTime = millis();
      alarmButton = false;
      tft.fillRect(0, 0, 320, 20, BLACK);
      tft.fillRect((54 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
      TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
      Serial.println("Alarm Acknowledged");
  }
}

// Gets an unsigned int from the Uno
unsigned int getSerialUInt() {
  while(Serial1.available() == 0) {}
  return Serial1.read();
}
// Calls on the Uno to get the temperature
int serialValue;
unsigned int getSerialTemp() {
  if(tempCheck) {
    annonciationCounter++;
    Serial1.write(0x00);
    tempCheck = false;
    serialValue = getSerialUInt();
    Serial.print("Temperature = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
// Calls on the Uno to get the systolic pressure
unsigned int getSysPress() {
  if(sysCheck){
    annonciationCounter++;
    Serial1.write(0x01);
    sysCheck = false;
    serialValue = getSerialUInt();
    Serial.print("Systolic = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
// Calls on the Uno to get the diastolic pressure
unsigned int getDiasPress() {
  if(diasCheck) {
    annonciationCounter++;
    Serial1.write(0x02);
    diasCheck = false;
    serialValue = getSerialUInt();
    Serial.print("Diastolic = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
// Calls on the Uno to get the pulse rate
unsigned int getPulseRate() {
  if(pulseCheck) {
    annonciationCounter++;
    Serial1.write(0x03);
    pulseCheck = false;
    serialValue = getSerialUInt();
    Serial.print("Pulse = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
