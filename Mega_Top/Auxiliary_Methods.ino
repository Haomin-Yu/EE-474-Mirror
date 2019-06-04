/*
 * Function name: Auxiliary_Methods
 * Function inputs: warning data struct to tell if there are any warnings or alarms, as well as touchscreen data
 * Function outputs: contains many miscillanious methods mentioned below such as tft labels and data from touchscreen.
 * Function description: Creatues many miscillanious methods to be used throughout
 *                       the rest of out programs so it can be cleaner, easier to understand, and easier to call.
 *                       this includes the touchscreen funcitonality
 *                       
 * Author: Haomin Yu and Nathan Ness
 *
 */

//includes rawStructs file
extern "C" {
  #include "rawStructs.h"
  #include "NetworkCommunication.h"
}

//includes all needed data structs, computational booleans, and variables needed to keep time.
extern MeasureDataStruct MeasureData;
extern WarningAlarmDataStruct WarningAlarmData;
extern TFTKeypadDataStruct KeypadData;
extern bool newTempComputed;
extern bool newBloodPressComputed;
extern bool newPulseRateComputed;
extern bool newRespirationComputed;
extern const unsigned long BUTTON_TIME;
extern unsigned long previousTime;
extern bool pulseBlink;
extern bool tempBlink;
extern bool bpBlink;

#define MINPRESSURE  10     //minimum pressure for a tft touch to be acknowledged.
#define MAXPRESSURE  1000   //maximum pressure for a tft touch to be acknowledged.

bool alarmButton = false;   //boolean used to keep track of if alarm button was pressed.
int pulseColor;
int tempColor;
int sysColor;
int diasColor;

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
  TFT_Write(GREEN, 10, 48,  "   B.P     ->        mmHg"); 
  TFT_Write(GREEN, 10, 73,  "Resp. Rate ->        RR"); 
  TFT_Write(GREEN, 10, 98,  "Pulse Rate ->        BPM"); 
  TFT_Write(GREEN, 10, 123, "Battery    ->");
  tft.fillRect(10, 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((12 + BUTTONWIDTH), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((14 + BUTTONWIDTH * 2), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((16 + BUTTONWIDTH * 3), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((54 + BUTTONWIDTH), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((16 + BUTTONWIDTH * 3), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  TFT_Write(RED, 12, 175, " Temp.");
  TFT_Write(RED, (14 + BUTTONWIDTH), 175, " B.P.");
  TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " R.R.");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
  TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
  TFT_Write(RED, 12, 217, " Blank");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 217, "Blank");
}
// Updates the measurement values(Erases the previous value)
void updateMeasurements(double tempCorrected, 
                        double systolicPressCorrected,
                        double diastolicPressCorrected,
                        double pulseRateCorrected,
                        double respirationCorrected,
                        double ekgCorrected,
                        unsigned short batteryState) {
  // Updating the measurements as well as decides the color at which each measurement should be displayed.
  bool newBatteryUpdate = newTempComputed || newBloodPressComputed || newPulseRateComputed;
  
  //temperature color and data display
  if(newTempComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed)) || (tempBlink && (((tempCorrected > 39.7) && (tempCorrected < 43.4)) || ((tempCorrected < 34.3) && (tempCorrected > 30.7))))) {          
    if(*WarningAlarmData.tempOutOfRange && ((tempCorrected > 43.4) || (tempCorrected < 30.7)) && (annonciationCounter > 4)) {
      tempColor = RED;
    } else if(*WarningAlarmData.tempOutOfRange) {
      if (((tempCorrected > 39.7) && (tempCorrected < 43.4)) || ((tempCorrected < 34.3) && (tempCorrected > 30.7))) {
          if (tempColor == YELLOW) {
            tempColor = BLACK;
          } else {
            tempColor = YELLOW;
          }
      } else {
      tempColor = YELLOW;
      }
    } else {
      tempColor = GREEN;
    }
    tft.fillRect(170, 23, 85, 24, BLACK);
    TFT_Write(tempColor, 170, 23, (String)tempCorrected);
    tempBlink = false;
  }
  
  //blood pressure color and data display
  if(newBloodPressComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed)) || (bpBlink && (((systolicPressCorrected > 136.5) && (systolicPressCorrected < 156)) || ((systolicPressCorrected < 114) && (systolicPressCorrected > 96)) || ((diastolicPressCorrected > 84) && (diastolicPressCorrected < 96)) || ((diastolicPressCorrected < 66.5) && (diastolicPressCorrected > 56))))) {     
    if(*WarningAlarmData.bpOutOfRange && ((systolicPressCorrected > 156) || (systolicPressCorrected < 96)) && (annonciationCounter > 4)) {
      sysColor = RED;
    } else if (*WarningAlarmData.bpOutOfRange) {
      if (((systolicPressCorrected > 136.5) && (systolicPressCorrected < 156)) || ((systolicPressCorrected < 114) && (systolicPressCorrected > 96))) {
          if (sysColor == YELLOW) {
            sysColor = BLACK;
          } else {
            sysColor = YELLOW;
          }
      } else {
        sysColor = YELLOW;
      }
    } else {
      sysColor = GREEN;
    }
    if(*WarningAlarmData.bpOutOfRange && ((diastolicPressCorrected > 96) || (diastolicPressCorrected < 56)) && (annonciationCounter > 4)) {
      diasColor = RED;
    } else if(*WarningAlarmData.bpOutOfRange) {
      if (((diastolicPressCorrected > 84) && (diastolicPressCorrected < 96)) || ((diastolicPressCorrected < 66.5) && (diastolicPressCorrected > 56))) {
          if (diasColor == YELLOW) {
            diasColor = BLACK;
          } else {
            diasColor = YELLOW;
          }
      } else {
        diasColor = YELLOW;
      }
    } else {
      diasColor = GREEN;
    }
    tft.fillRect(170, 48, 85, 24, BLACK);
    String bloodPressure = (String)(unsigned int)systolicPressCorrected + "/" 
                         + (String)(unsigned int)diastolicPressCorrected;
    TFT_Write(sysColor, 170, 48, bloodPressure);
    bpBlink = false;
  }

  //respiration color and data display
  if(newRespirationComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed))) {  
     int respirationColor;
     if(*WarningAlarmData.respOutOfRange && ((respirationCorrected > 28) || (respirationCorrected < 10)) && (annonciationCounter > 4)) {
       respirationColor = RED;
     } else if (*WarningAlarmData.respOutOfRange) {
       respirationColor = YELLOW;
     } else {
       respirationColor = GREEN;
     }
     
     tft.fillRect(170, 73, 85, 24, BLACK);
     TFT_Write(respirationColor, 170, 73, (String)(int)respirationCorrected); 
  }
  
  //pulse color and data display
  if(newPulseRateComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed)) || (pulseBlink && (((pulseRateCorrected > 105) && (pulseRateCorrected < 115)) || ((pulseRateCorrected < 57) && (pulseRateCorrected > 51))))) {
     if(*WarningAlarmData.pulseOutOfRange && ((pulseRateCorrected > 115) || (pulseRateCorrected < 51)) && (annonciationCounter > 4)) {
      pulseColor = RED;
    } else if(*WarningAlarmData.pulseOutOfRange) {
       if (((pulseRateCorrected > 105) && (pulseRateCorrected < 115)) || ((pulseRateCorrected < 57) && (pulseRateCorrected > 51))) {
          if (pulseColor == YELLOW) {
            pulseColor = BLACK;
          } else {
            pulseColor = YELLOW;
          }
      } else {
        pulseColor = YELLOW;
      }
     } else {
       pulseColor = GREEN;
     }
     tft.fillRect(170, 98, 85, 24, BLACK);
     TFT_Write(pulseColor, 170, 98,  (String)(int)pulseRateCorrected); 
     newPulseRateComputed = false;
     newBloodPressComputed = false;
     newRespirationComputed = false;
     newTempComputed = false;
     pulseBlink = false;
  }

  // EKG and data display
  // TODO
  
  //battery color and data display
  if(newBatteryUpdate) {                          
     int battColor;
     if(*WarningAlarmData.batteryOutOfRange && (annonciationCounter > 4)) {
       battColor = RED;
     } else if (*WarningAlarmData.batteryOutOfRange) {
       battColor = YELLOW;
     } else {
       battColor = GREEN;
     }
     tft.fillRect(170, 123, 85, 24, BLACK);
     TFT_Write(battColor, 170, 123, (String)batteryState); 
  }
}

//This is used to check for any buttons pressed and assign coordinating pointers and flags based off of buttons pressed
void touchScreen() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (!(*KeypadData.alarmAcknowledge == 0) && alarmCheck) {   //if an alarm is meant to go off and it is not acknowledged, it will display an alarm warning
    if (annonciationCounter > 4) {
      tft.fillRect(0, 0, 320, 20, RED);
      TFT_Write(WHITE, 5, 3, "ALARM ACTIVE  ALARM ACTIVE");
      alarmCheck = false;
    }
  }

  //maps our tft display based off of the fact that we have a horizontal display.
  p.x = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);
  //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = (tft.height()-map(p.y, TS_MINX, TS_MAXX, tft.width(), 0));
  //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {                                                                       //checks to see if the tft is pressed anywhere
     if((p.x < (BUTTONHEIGHT + 160)) && (p.x > 160) && (*KeypadData.alarmAcknowledge == 0)) {                         //checks to see if it alligns with the buttons horizontal axis.
        if(((tft.height()-p.y) < (BUTTONWIDTH + 10)) && ((tft.height()-p.y) > 10)) {                                  //checks to see if the vertical axis for temp was pressed.
           tempCheck = true;
           *MeasureData.measurementSelection = 1; //assigns button selected data
           *KeypadData.localMeasurementSelection = 1; 
           tft.fillRect(10, 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);//changes color to represent a button press
           TFT_Write(RED, 12, 175, " Temp.");
        }
        else if(((tft.height()-p.y) < (12 + BUTTONWIDTH * 2)) && ((tft.height()-p.y) > (12 + BUTTONWIDTH))) {         //checks to see if the vertical axis for sys was pressed.
           bloodPressCheck = true;
           *MeasureData.measurementSelection = 2;//assigns button selected data
           *KeypadData.localMeasurementSelection = 2; 
           tft.fillRect((12 + BUTTONWIDTH), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);//changes color to represent a button press
           TFT_Write(RED, (14 + BUTTONWIDTH), 175, " B.P.");
        } 
        else if(((tft.height()-p.y) < (14 + BUTTONWIDTH * 3)) && ((tft.height()-p.y) > (14 + BUTTONWIDTH * 2))) {     //checks to see if the vertical axis for dias was pressed.
           respirationCheck = true;
           *MeasureData.measurementSelection = 3; //assigns button selected data
           *KeypadData.localMeasurementSelection = 3;
           tft.fillRect((14 + BUTTONWIDTH * 2), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);//changes color to represent a button press
           TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " R.R.");
        } 
        else if(((tft.height()-p.y) < (16 + BUTTONWIDTH * 4)) && ((tft.height()-p.y) > (16 + BUTTONWIDTH * 3))) {     //checks to see if the vertical axis for pulse was pressed.
           pulseCheck = true;
           *MeasureData.measurementSelection = 4; //assigns button selected data
           *KeypadData.localMeasurementSelection = 4;
           tft.fillRect((16 + BUTTONWIDTH * 3), 160, BUTTONWIDTH, BUTTONHEIGHT, BLUE);//changes color to represent a button press
           TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
        }
     }
     else if((p.x < (BUTTONHEIGHT + 202)) && (p.x > 202)) {                                                           //checks to see if it alligns with the buttons horizontal axis.
      if(((tft.height()-p.y) < ((BUTTONWIDTH *2) + 54)) && ((tft.height()-p.y) > (54 + BUTTONWIDTH))) {               //checks to see if the vertical axis for alarm was pressed.
           
            annonciationCounter = 0;                                              //sets counter used to see how long its been since an acknowledgment to zero
           *KeypadData.alarmAcknowledge = 0;                                      //sets alarm acknowledgement
           alarmButton = true;                                                    //says alarm button was pressed
           tft.fillRect((54 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, BLUE);//changes color to represent a button press
           TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
           
        } else if (((tft.height()-p.y) < ((BUTTONWIDTH) + 10)) && ((tft.height()-p.y) > 10)) {                        //checks to see if the vertical axis for blank was pressed.
          tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLUE);             //changes color to represent a button press
          TFT_Write(RED, 12, 217,"Blank");
          
        } else if (((tft.height()-p.y) < ((16 + BUTTONWIDTH * 4))) && ((tft.height()-p.y) > 16)) {                    //checks to see if the vertical axis for blank was pressed.
          tft.fillRect((16 + BUTTONWIDTH * 3), 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLUE);//changes color to represent a button press
          TFT_Write(RED, (18 + BUTTONWIDTH * 3), 217,"Blank");
        }
     } 
  } else if (alarmButton && ((millis() - previousTime) > BUTTON_TIME)){                                               //if the alarm button was pressed and its time to look at the 
      previousTime = millis();                                                                                        //buttons pressed we will check them
      alarmButton = false;
      tft.fillRect(0, 0, 320, 20, BLACK);                                                            //clears screen to normal and buttons back to unpressed.
      tft.fillRect((54 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
      TFT_Write(RED, (60 + BUTTONWIDTH), 217, "Alarm");
      Serial.println("Alarm Acknowledged");                                                          //will print in serial that alarm was acknowledged
  }
}

// Gets an unsigned int from the Uno
unsigned int getSerialUInt(byte task) {
  unsigned int measuredInt = 0;
  while(Serial1.available() == 0) {}
  delay(10);
  if(Serial1.available() == 5) {
    // Throwing away start byte
    Serial1.read();
    // Throwing away task byte
    Serial1.read();
    // Throwing away requested byte
    Serial1.read();
    // Grabbing data byte
    measuredInt = Serial1.read();
    // Throwing away end byte
    Serial1.read();
    // Returning the value
    return measuredInt;
  }
  else if(Serial1.available() == 6) {
    // Throwing away start byte
    Serial1.read();
    // Throwing away task byte
    Serial1.read();
    // Throwing away requested byte
    Serial1.read();
    // Grabbing data byte
    byte data1 = Serial1.read();
    byte data2 = Serial1.read();
    // Throwing away end byte
    Serial1.read();
    // Returning the value
    measuredInt = (data1 << 8) | data2;
    return measuredInt;
  }
  else if(Serial1.available() == 3) {
    byte startByte = Serial1.read();
    byte taskByte  = Serial1.read();
    byte requestByte = Serial1.read();
    // Checking if it is valid EKG identifier
    if((startByte == 0xE7) && (taskByte == 0xFF) && (requestByte == 0xFF)) {
       double* arrayPointer = MeasureData.EKGRawBuf;
       Serial1.write(0xAA);
       for(int times = 0; times < 4; times++) {
         for(int i = 0; i < 64; i++) {
           arrayPointer[i] = Serial1.read();
         }
         Serial1.write(0xAA);
       }
       // Throwing away end byte
       Serial1.read();
       return arrayPointer;
    }
    else { // Nope. Flush
      while(Serial1.available() != 0) {
        Serial1.read();
      }
    }
  }
  else { // Flush
    while(Serial1.available() != 0) {
      Serial1.read();
    }
  }
}
// Calls on the Uno to get the temperature & prints information in serial monitor
int serialValue; //used to hold the value recieved through serial communication
unsigned int getSerialTemp() {
  if(tempCheck) {
    tempCheck = false;
    annonciationCounter++;
    sendLocalMessage(0xE7, 0x00, 0xFF, 0xFF, 0xDB);
    serialValue = getSerialUInt(0x00);
    Serial.print("Locally Received Temperature = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
// Calls on the Uno to get the diastolic pressure & prints information in serial monitor
unsigned int getBloodPress() {
  if(bloodPressCheck) {
    bloodPressCheck = false;
    annonciationCounter++;
    sendLocalMessage(0xE7, 0x01, 0xFF, 0xFF, 0xDB);
    serialValue = getSerialUInt(0x01);
    Serial.print("Locally Received Blood Pressure = ");
    Serial.print((unsigned int)serialValue >> 8, DEC);
    Serial.print("/");
    Serial.println(serialValue & 0xFF, DEC);
    return serialValue;
  }
}
// Calls on the Uno to get the respiration & prints information in serial monitor
unsigned int getRespiration() {
  if(respirationCheck) {
    respirationCheck = false;
    annonciationCounter++;
    sendLocalMessage(0xE7, 0x02, 0xFF, 0xFF, 0xDB);
    serialValue = getSerialUInt(0x02);
    Serial.print("Locally Received Respiration = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
// Calls on the Uno to get the pulse rate & prints information in serial monitor
unsigned int getPulseRate() {
  if(pulseCheck) {
    pulseCheck = false;
    annonciationCounter++;
    sendLocalMessage(0xE7, 0x03, 0xFF, 0xFF, 0xDB);
    serialValue = getSerialUInt(0x03);
    Serial.print("Locally Received Pulse = ");
    Serial.println(serialValue);
    return serialValue;
  }
}
// Calls on the Uno to get the EKG measurement & prints information in serial monitor
void getEKG() {
  if(ekgCheck) {
     ekgCheck = false;
     annonciationCounter++;
     sendLocalMessage(0xE7, 0x04, 0xFF, 0xFF, 0xDB);
     getSerialUInt(0x04);
     Serial.println("Locally Received EKG measurements..");
  }
}
