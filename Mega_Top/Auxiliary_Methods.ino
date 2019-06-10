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


#define BUZZER_PIN 52

//includes all needed data structs, computational booleans, and variables needed to keep time.
extern MeasureDataStruct MeasureData;
extern WarningAlarmDataStruct WarningAlarmData;
extern TFTKeypadDataStruct KeypadData;
extern bool newTempComputed;
extern bool newBloodPressComputed;
extern bool newPulseRateComputed;
extern bool newRespirationComputed;
extern bool newEKGComputed;
extern const unsigned long BUTTON_TIME;
extern unsigned long previousTime;
extern bool pulseBlink;
extern bool tempBlink;
extern bool bpBlink;
extern bool displayOn;

#define MINPRESSURE  10     //minimum pressure for a tft touch to be acknowledged.
#define MAXPRESSURE  1000   //maximum pressure for a tft touch to be acknowledged.

bool alarmButton = false;   //boolean used to keep track of if alarm button was pressed.
int pulseColor;
int tempColor;
int alarmColor;
int sysColor;
int diasColor;
int tempWarningCount;
int bpWarningCount;
int pulseWarningCount;
int respWarningCount;
int batteryWarningCount;
bool tempCountIncr = true;
bool bpCountIncr = true;
bool pulseCountIncr = true;
bool respCountIncr = true;
bool batteryCountIncr = true;
long eightHourTimer = millis();

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
  TFT_Write(GREEN, 10, 10,  "Body.Temp  ->        C");
  TFT_Write(GREEN, 10, 35,  "   B.P     ->        mmHg"); 
  TFT_Write(GREEN, 10, 60,  "Resp. Rate ->        BPM"); 
  TFT_Write(GREEN, 10, 85,  "Pulse Rate ->        BPM"); 
  TFT_Write(GREEN, 10, 110, " Peak EKG  ->        Hz"); 
  TFT_Write(GREEN, 10, 135, "Battery    ->");
  tft.fillRect(10, 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((12 + BUTTONWIDTH), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((14 + BUTTONWIDTH * 2), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((16 + BUTTONWIDTH * 3), 160, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((12 + BUTTONWIDTH), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  tft.fillRect((14 + BUTTONWIDTH * 2), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);
  TFT_Write(RED, 12, 175, " Temp.");
  TFT_Write(RED, (14 + BUTTONWIDTH), 175, " B.P.");
  TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " R.R.");
  TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
  TFT_Write(RED, (18 + BUTTONWIDTH), 217, "Alarm");
  TFT_Write(RED, 12, 217, " EKG");
  TFT_Write(RED, (14 + BUTTONWIDTH * 2), 217, "Traffic");
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

  if(!(*KeypadData.alarmAcknowledge == 0) && (alarmColor == BLACK) && displayOn) {
      tft.fillRect((16 + BUTTONWIDTH * 3), 202, (BUTTONWIDTH), (BUTTONHEIGHT), RED);
      TFT_Write(WHITE, (18 + BUTTONWIDTH * 3), 204, "ALARM");
      TFT_Write(WHITE, (18 + BUTTONWIDTH * 3), 224, "ACT.");
      alarmColor = RED;
  }

  //temperature color and data display
  if(newTempComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed)) || (tempBlink && (((tempCorrected > 39.7) && (tempCorrected < 43.4)) || ((tempCorrected < 34.3) && (tempCorrected > 30.7))))) {          
    if(*WarningAlarmData.tempOutOfRange && ((tempCorrected > 43.4) || (tempCorrected < 30.7)) && (annonciationCounter > 4)) {
      tempColor = RED;
      if(tempCountIncr) {
        tempWarningCount++;
        tempCountIncr = false;
      }
    } else if(*WarningAlarmData.tempOutOfRange) {
      if (((tempCorrected > 39.7) && (tempCorrected < 43.4)) || ((tempCorrected < 34.3) && (tempCorrected > 30.7))) {
          if ((!displayOn) || (tempColor == YELLOW)) {
            tempColor = BLACK;
            if(tempCountIncr) {
              tempWarningCount++;
              tempCountIncr = false;
            }
          } else {
            tempColor = YELLOW;
            if(tempCountIncr) {
              tempWarningCount++;
              tempCountIncr = false;
            }
          }
      } else {
      tempColor = YELLOW;
      if(tempCountIncr) {
        tempWarningCount++;
        tempCountIncr = false;
      }
      }
    } else {
      tempColor = GREEN;
      if(!tempCountIncr) {
        tempCountIncr = true;
      }
    }
    tft.fillRect(170, 10, 85, 24, BLACK);
    TFT_Write(tempColor, 170, 10, (String)tempCorrected);
    tempBlink = false;
  }
  
  //blood pressure color and data display
  if(newBloodPressComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed)) || (bpBlink && (((systolicPressCorrected > 136.5) && (systolicPressCorrected < 156)) || ((systolicPressCorrected < 114) && (systolicPressCorrected > 96)) || ((diastolicPressCorrected > 84) && (diastolicPressCorrected < 96)) || ((diastolicPressCorrected < 66.5) && (diastolicPressCorrected > 56))))) {     
    if(*WarningAlarmData.bpOutOfRange && ((systolicPressCorrected > 156) || (systolicPressCorrected < 96)) && (annonciationCounter > 4)) {
      sysColor = RED;
      if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
    } else if (*WarningAlarmData.bpOutOfRange) {
      if (((systolicPressCorrected > 136.5) && (systolicPressCorrected < 156)) || ((systolicPressCorrected < 114) && (systolicPressCorrected > 96))) {
          if ((!displayOn) || (sysColor == YELLOW)) {
            sysColor = BLACK;
            if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
          } else {
            sysColor = YELLOW;
            if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
          }
      } else {
        sysColor = YELLOW;
        if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
      }
    } else {
      sysColor = GREEN;
      if(!bpCountIncr) {
        bpCountIncr = true;
      }
    }
    if(*WarningAlarmData.bpOutOfRange && ((diastolicPressCorrected > 96) || (diastolicPressCorrected < 56)) && (annonciationCounter > 4)) {
      diasColor = RED;
      if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
    } else if(*WarningAlarmData.bpOutOfRange) {
      if (((diastolicPressCorrected > 84) && (diastolicPressCorrected < 96)) || ((diastolicPressCorrected < 66.5) && (diastolicPressCorrected > 56))) {
          if ((!displayOn) || (diasColor == YELLOW)) {
            diasColor = BLACK;
            if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
          } else {
            diasColor = YELLOW;
            if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
          }
      } else {
        diasColor = YELLOW;
        if(bpCountIncr) {
        bpWarningCount++;
        bpCountIncr = false;
      }
      }
    } else {
      diasColor = GREEN;
      if(!bpCountIncr) {
        bpCountIncr = true;
      }
    }
    tft.fillRect(170, 35, 85, 24, BLACK);
    String bloodPressure = (String)(unsigned int)systolicPressCorrected + "/" 
                         + (String)(unsigned int)diastolicPressCorrected;
    TFT_Write(sysColor, 170, 35, bloodPressure);
    bpBlink = false;
  }

  //respiration color and data display
  if(newRespirationComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed))) {  
     int respirationColor;
     if(*WarningAlarmData.respOutOfRange && ((respirationCorrected > 28) || (respirationCorrected < 10)) && (annonciationCounter > 4)) {
       respirationColor = RED;
       if(respCountIncr) {
        respWarningCount++;
        respCountIncr = false;
      }
     } else if (*WarningAlarmData.respOutOfRange) {
       respirationColor = YELLOW;
       if(respCountIncr) {
        respWarningCount++;
        respCountIncr = false;
      }
     } else {
       respirationColor = GREEN;
       if(respCountIncr) {
        respCountIncr = true;
      }
     }
     
     tft.fillRect(170, 60, 85, 24, BLACK);
     TFT_Write(respirationColor, 170, 60, (String)(int)respirationCorrected); 
  }
  
  //pulse color and data display
  if(newPulseRateComputed || (alarmCheck && (newBloodPressComputed || newRespirationComputed || newPulseRateComputed || newTempComputed)) || (pulseBlink && (((pulseRateCorrected > 105) && (pulseRateCorrected < 115)) || ((pulseRateCorrected < 57) && (pulseRateCorrected > 51))))) {
     if(*WarningAlarmData.pulseOutOfRange && ((pulseRateCorrected > 115) || (pulseRateCorrected < 51)) && (annonciationCounter > 4)) {
      pulseColor = RED;
      if(pulseCountIncr) {
        pulseWarningCount++;
        pulseCountIncr = false;
      }
    } else if(*WarningAlarmData.pulseOutOfRange) {
       if (((pulseRateCorrected > 105) && (pulseRateCorrected < 115)) || ((pulseRateCorrected < 57) && (pulseRateCorrected > 51))) {
          if ((!displayOn) || (pulseColor == YELLOW)) {
            pulseColor = BLACK;
            if(pulseCountIncr) {
        pulseWarningCount++;
        pulseCountIncr = false;
      }
          } else {
            pulseColor = YELLOW;
            if(pulseCountIncr) {
        pulseWarningCount++;
        pulseCountIncr = false;
      }
          }
      } else {
        pulseColor = YELLOW;
        if(pulseCountIncr) {
        pulseWarningCount++;
        pulseCountIncr = false;
      }
      }
     } else {
       pulseColor = GREEN;
       if(!pulseCountIncr) {
        pulseCountIncr = true;
      }
     }
     tft.fillRect(170, 85, 85, 24, BLACK);
     TFT_Write(pulseColor, 170, 85,  (String)(int)pulseRateCorrected); 
     newPulseRateComputed = false;
     newBloodPressComputed = false;
     newRespirationComputed = false;
     newTempComputed = false;
     pulseBlink = false;
  }

  // EKG and data display
  if(newEKGComputed) {
    int ekgColor = GREEN;
    tft.fillRect(170, 110, 85, 24, BLACK);
    TFT_Write(ekgColor, 170, 110, (String)(int)ekgCorrected);
    newEKGComputed = false;
  }
  
  //battery color and data display
  if(newBatteryUpdate) {                          
     int battColor;
     if(*WarningAlarmData.batteryOutOfRange && (annonciationCounter > 4)) {
       battColor = RED;
       if(batteryCountIncr) {
        batteryWarningCount++;
        batteryCountIncr = false;
      }
     } else if (*WarningAlarmData.batteryOutOfRange) {
       battColor = YELLOW;
       if(batteryCountIncr) {
        batteryWarningCount++;
        batteryCountIncr = false;
      }
     } else {
       battColor = GREEN;
       if(!batteryCountIncr) {
        batteryCountIncr = true;
      }
     }
     tft.fillRect(170, 135, 85, 24, BLACK);
     TFT_Write(battColor, 170, 135, (String)batteryState); 
  }
}

//This is used to check for any buttons pressed and assign coordinating pointers and flags based off of buttons pressed
void touchScreen() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if((millis() - eightHourTimer) > 28800000) {
    tempWarningCount = 0;
    bpWarningCount = 0;
    pulseWarningCount = 0;
    respWarningCount = 0;
    batteryWarningCount = 0;
    eightHourTimer = millis();
  }

  //maps our tft display based off of the fact that we have a horizontal display.
  p.x = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);
  //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = (tft.height()-map(p.y, TS_MINX, TS_MAXX, tft.width(), 0));
  //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

  if (displayOn && p.z > MINPRESSURE && p.z < MAXPRESSURE) {                                                                       //checks to see if the tft is pressed anywhere
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
      if(((tft.height()-p.y) < ((BUTTONWIDTH *2) + 12)) && ((tft.height()-p.y) > (12 + BUTTONWIDTH))) {               //checks to see if the vertical axis for alarm was pressed.
           
            annonciationCounter = 0;                                              //sets counter used to see how long its been since an acknowledgment to zero
           *KeypadData.alarmAcknowledge = 0;                                      //sets alarm acknowledgement
           alarmButton = true;                                                    //says alarm button was pressed
           tft.fillRect((12 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, BLUE);//changes color to represent a button press
           TFT_Write(RED, (18 + BUTTONWIDTH), 217, "Alarm");
           
        } else if (((tft.height()-p.y) < ((BUTTONWIDTH) + 10)) && ((tft.height()-p.y) > 10) && (*KeypadData.alarmAcknowledge == 0)) {                        //checks to see if the vertical axis for blank was pressed.
          ekgCheck = true;
           *MeasureData.measurementSelection = 5; //assigns button selected data
           *KeypadData.localMeasurementSelection = 5;
          tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLUE);             //changes color to represent a button press
          TFT_Write(RED, 12, 217," EKG");
          
        } else if (((tft.height()-p.y) < ((14 + BUTTONWIDTH * 3))) && ((tft.height()-p.y) > (14 + BUTTONWIDTH * 2)) && (*KeypadData.alarmAcknowledge == 0)) {                    //checks to see if the vertical axis for blank was pressed.
          tft.fillRect((14 + BUTTONWIDTH * 2), 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLUE);//changes color to represent a button press
          TFT_Write(RED, (16 + BUTTONWIDTH * 2), 217,"Traffic");
          unsigned int i;
          for(i=0;i<75;i++) {
            digitalWrite(BUZZER_PIN,HIGH);
            delay(4);//wait for 1ms
            digitalWrite(BUZZER_PIN,LOW);
            delay(4);//wait for 1ms
          }
          //output another frequency
          for(i=0;i<200;i++) {
            digitalWrite(BUZZER_PIN,HIGH);
            delay(1);
            digitalWrite(BUZZER_PIN,LOW);
            delay(1);
          }
          for(i=0;i<200;i++) {
            digitalWrite(BUZZER_PIN,HIGH);
            delay(2);
            digitalWrite(BUZZER_PIN,LOW);
            delay(2);
          }
          for(i=0;i<175;i++) {
            digitalWrite(BUZZER_PIN,HIGH);
            delay(3);
            digitalWrite(BUZZER_PIN,LOW);
            delay(3);
          }
          tft.fillRect((14 + BUTTONWIDTH * 2), 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);//changes color to represent a button press
          TFT_Write(RED, (16 + BUTTONWIDTH * 2), 217,"Traffic");
        }
     } 
  } else if (alarmButton && ((millis() - previousTime) > BUTTON_TIME)){                                               //if the alarm button was pressed and its time to look at the 
      previousTime = millis();                                                                                        //buttons pressed we will check them
      alarmButton = false;
      tft.fillRect((16 + BUTTONWIDTH * 3), 202, (BUTTONWIDTH), (BUTTONHEIGHT), BLACK);                                                            //clears screen to normal and buttons back to unpressed.
      alarmColor = BLACK;
      tft.fillRect((12 + BUTTONWIDTH), 202, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
      TFT_Write(RED, (18 + BUTTONWIDTH), 217, "Alarm");
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
static unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
double* getEKG() {
  if(ekgCheck) {
     ekgCheck = false;
     annonciationCounter++;
     unsigned long microseconds = 0;
     double* realArrayPointer = MeasureData.EKGRawBuf;
     double* imagArrayPointer = ekg_imag_INIT;
     for(int i=0; i<SAMPLES; i++) {
        microseconds = micros();
        realArrayPointer[i] = analogRead(EKG) - 610;
        imagArrayPointer[i] = 0;
        while(micros() < (microseconds + sampling_period_us)){}
    }
     Serial.println("Locally Received EKG measurements..");
     return realArrayPointer;
  }
}
