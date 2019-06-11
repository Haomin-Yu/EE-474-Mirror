/*
 * Function name: NetworkCommunication
 * Function inputs: This file inputs the compute and rawstruct files in order to gather the needed information for
 *                  the communication between the local and remote networks. It also inputs data from the remote network
 * Function outputs: This outputs data to the remote communication so it can be displayed remotely for the doctor.
 * Function description: This function is used to communicate between the local arduino network and the used remote puTTy network.
 * Author: Haomin Yu and Nathan Ness
 */

 //adds the compute and rawstruct files to be used in NetworkCommunications
#include "compute.h"
#include "rawStructs.h"
#include <string.h>

//externs all needed variables from other files
extern bool newTempComputed;
extern bool newBloodPressComputed;
extern bool newPulseRateComputed;
extern bool newRespirationComputed;
extern bool newEKGComputed;
extern int alarmColor;
extern int tempWarningCount;
extern int bpWarningCount;
extern int pulseWarningCount;
extern int respWarningCount;
extern int batteryWarningCount;

// Global structs
extern DisplayDataStruct DisplayData;
extern WarningAlarmDataStruct WarningAlarmData;

// Function prototypes
void remoteCommunication();
void sendLocalMessage(byte startByte, byte task, byte ID, byte data, byte endByte);
void sendRemoteMessage(byte startByte, byte task, byte ID, int data, byte endByte);
byte toUpper(byte input);

// Network Identifiers
const static byte I = 0x49;
const static byte E = 0x45;
const static byte S = 0x53;
const static byte P = 0x50;
const static byte D = 0x44;
const static byte M = 0x4D;
const static byte W = 0x57;

// Measure Definitions
const static byte measureTemperatureFunc   = 0x00;
const static byte measureBloodPressureFunc = 0x01;
const static byte measureRespirationFunc   = 0x02;
const static byte measurePulseRateFunc     = 0x03;
const static byte measureEKGFunc           = 0x04;

// Class variables
static String remoteDataMessage = "";

/*
 * Handles the remote communication from and to the system
 */
static bool initialized = false;
static bool displayOn   = true;
static bool displaySwitch = true;
void remoteCommunication() {
  if(Serial.available() > 0) {
    byte command = toUpper(Serial.read());
    if(command == I) {
      // Initialize Network
      if(initialized) {
        Serial.println("I: Re-initializing network..");
      }
      else {
        Serial.println("I: Initializing network..");
      }
      initialized = true;
      Serial.println("Done");
      Serial.println("Product Name: Doctor at Your Fingertips");
      Serial.println("Patient's Name: Warren Buffett");
      Serial.println("Doctor's Name: Dr.James Peckol");
    }
    else if (!initialized){
      Serial.println("E: Network is not initialized!");
      Serial.println("Try using the command 'I'");
    }
    else if(command == E) {
      // Show Error Message
      Serial.println("E: This an error response when incorrect or non-existent commands are given");
    }
    else if(command == S) {
      // Start Measurement
      Serial.println("S: Measurement mode enabled.");
      // Getting temperature
      if(waitResponseSP()) {
        Serial.println("S: Measuring Temperature...");
        tempCheck = true;
        int measuredTempValue = getSerialTemp();
        Serial.print("Remotely Received Temperature = "); Serial.println((int)computeTemp(measuredTempValue));
      }
      else {
        Serial.println("P: Measurement has stopped");
        goto stopMeasurement;
      }
      if(waitResponseSP()) {
        //getting blood pressure
        Serial.println("S: Measuring Blood Pressure...");
        bloodPressCheck = true;
        int measuredBPData = getBloodPress();
        Serial.print("Remotely Received Blood Pressure = ");
        Serial.print((int)computeSys((unsigned int)measuredBPData >> 8));
        Serial.print("/");
        measuredBPData = (int)computeDias(measuredBPData & 0xFF);
        Serial.println(measuredBPData);
      }
      else {
        Serial.println("P: Measurement has stopped");
        goto stopMeasurement;
     }
    if(waitResponseSP()) {
      //getting respiration
      Serial.println("S: Measuring Respiration...");
        respirationCheck = true;
        int measuredRespValue = getRespiration();
        Serial.print("Remotely Received Respiration = "); Serial.println((int)computeRespiration(measuredRespValue));
      }
      else {
        Serial.println("P: Measurement has stopped");
        goto stopMeasurement;
      }
      if(waitResponseSP()) {
        //getting pulse rate
        Serial.println("S: Measuring Pulse Rate...");
        pulseCheck = true;
        int measuredPulseValue = getPulseRate();
        Serial.print("Remotely Received Pulse = "); Serial.println((int)computePr(measuredPulseValue));
      }
      else {
        Serial.println("P: Measurement has stopped");
        goto stopMeasurement;
      }
      if(waitResponseSP()) {
        //getting EKG
        ekgCheck = true;
        double* measuredEKGValue = getEKG();
        Serial.print("Remotely Received EKG = "); Serial.println((int)computeEKG(measuredEKGValue));
      }
      else {
        Serial.println("P: Measurement has stopped");
        goto stopMeasurement;
      }
      stopMeasurement:;
    }
    else if(command == P) {
      // Break from measurement
      Serial.println("E: No measurement is ongoing. Nothing to stop.");
    }
    else if(command == D) {
      // Toggles TFT display
      if(displayOn) {
        Serial.println("D: TFT Screen Off...");
        // Makes the whole screen black
        tft.fillScreen(BLACK);
        alarmColor = BLACK;
        displaySwitch = true;
      }
      else {
        //turns TFT screen back on
        Serial.println("D: TFT Screen On...");
        TFT_Write(GREEN, 10, 10,  "Body.Temp  ->        C");
        TFT_Write(GREEN, 10, 35,  "   B.P     ->        mmHg"); 
        TFT_Write(GREEN, 10, 60,  "Resp. Rate ->        BPM"); 
        TFT_Write(GREEN, 10, 85,  "Pulse Rate ->        BPM"); 
        TFT_Write(GREEN, 10, 110, " Peak EKG  ->        Hz"); 
        TFT_Write(GREEN, 10, 135, "Battery    ->");
        if (displaySwitch == true) {
          newTempComputed = true;
          newBloodPressComputed = true;
          newPulseRateComputed = true;
          newRespirationComputed = true;
          newEKGComputed = true;
          displaySwitch = false;
        }
        updateMeasurements((int)DisplayData.tempCorrectedBuf[*DisplayData.currentTemperatureIndex],
                           (int)DisplayData.bloodPressCorrectedBuf[*DisplayData.currentSysPressIndex],
                           (int)DisplayData.bloodPressCorrectedBuf[*DisplayData.currentDiasPressIndex],
                           (int)DisplayData.prCorrectedBuf[*DisplayData.currentPulseRateIndex],
                           (int)DisplayData.respirationCorrectedBuf[*DisplayData.currentRespirationIndex],
                           (int)DisplayData.EKGFreqBuf[*DisplayData.currentEKGIndex],
                           *DisplayData.batteryState);                     
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
      displayOn = !displayOn;
    }
    else if(command == M) {
      // Prints out information from 'DisplayData'
      String temp = (String)(int)DisplayData.tempCorrectedBuf[*DisplayData.currentTemperatureIndex];
      String sys  = (String)(int)DisplayData.bloodPressCorrectedBuf[*DisplayData.currentSysPressIndex];
      String dias = (String)(int)DisplayData.bloodPressCorrectedBuf[*DisplayData.currentDiasPressIndex];
      String pr   = (String)(int)DisplayData.prCorrectedBuf[*DisplayData.currentPulseRateIndex];
      String resp = (String)(int)DisplayData.respirationCorrectedBuf[*DisplayData.currentRespirationIndex];
      String ekg  = (String)(int)DisplayData.EKGFreqBuf[*DisplayData.currentEKGIndex];
      String batt = (String)*DisplayData.batteryState;
      Serial.println("M: Printing out most recent data:");
      Serial.print("* Temperature  = "); Serial.print(temp); Serial.print("\t C"); Serial.print("     Warning Count: ");  Serial.println(tempWarningCount);
      Serial.print("* Sys. Press.  = "); Serial.print(sys);  Serial.print("\t mmHg"); Serial.print("  Warning Count: "); Serial.println(bpWarningCount);
      Serial.print("* Dias. Press. = "); Serial.print(dias); Serial.print("\t mmHg"); Serial.print("  Warning Count: "); Serial.println(bpWarningCount);
      Serial.print("* Pulse Rate   = "); Serial.print(pr);   Serial.print("\t BPM"); Serial.print("   Warning Count: "); Serial.println(pulseWarningCount);
      Serial.print("* Resp. Rate   = "); Serial.print(resp); Serial.print("\t BPM"); Serial.print("   Warning Count: "); Serial.println(respWarningCount);
      Serial.print("* EKG          = "); Serial.print(ekg);  Serial.println("\t Hz");
      Serial.print("* Battery      = "); Serial.print(batt);  Serial.print("           Warning Count: "); Serial.println(batteryWarningCount);
    }
    else if(command == W) {
      // Prints out information from 'WarningAlarmData'
      Serial.println("W: Printing out most recent warning/alarm information:");
      Serial.print("* Temp:    ");
      //decides temps current alarm state
      int currentTempCorrected = (int)DisplayData.tempCorrectedBuf[*DisplayData.currentTemperatureIndex];
       if(*WarningAlarmData.tempOutOfRange && ((currentTempCorrected > 43.4) || (currentTempCorrected < 30.7))) {
        if((annonciationCounter > 4)) {
          Serial.println("Alarm Active");
        } else {
          Serial.println("Alarm Active (Acknowledged)");
        }
    } else if(*WarningAlarmData.tempOutOfRange) {
      if (((currentTempCorrected > 39.7) && (currentTempCorrected < 43.4)) || ((currentTempCorrected < 34.3) && (currentTempCorrected > 30.7))) {
          Serial.println("Warning (Blinking)");
      } else {
        Serial.println("Warning");
      }
    } else {
      Serial.println("Normal Range");
    } 
    
      Serial.print("* BP:      ");
       //decides blood pressures current alarm state
      int currentSysCorrected = (int)DisplayData.bloodPressCorrectedBuf[*DisplayData.currentSysPressIndex];
      int currentDiasCorrected = (int)DisplayData.bloodPressCorrectedBuf[*DisplayData.currentDiasPressIndex];
      if(*WarningAlarmData.bpOutOfRange && (((currentSysCorrected > 156) || (currentSysCorrected < 96)) || ((currentDiasCorrected > 96) || (currentDiasCorrected < 56)))) {
        if((annonciationCounter > 4)) {
          Serial.println("Alarm Active");
        } else {
          Serial.println("Alarm Active (Acknowledged)");
        }
    } else if(*WarningAlarmData.bpOutOfRange) {
      if ((((currentSysCorrected > 136.5) && (currentSysCorrected < 156)) || ((currentSysCorrected < 114) && (currentSysCorrected > 96))) || (((currentDiasCorrected > 84) && (currentDiasCorrected < 96)) || ((currentDiasCorrected < 66.5) && (currentDiasCorrected > 56)))) {
          Serial.println("Warning (Blinking)");
      } else {
        Serial.println("Warning");
      }
    } else {
      Serial.println("Normal Range");
    } 
    
      Serial.print("* Pulse:   ");
       //decides pulses current alarm state
       int currentPulseCorrected = (int)DisplayData.prCorrectedBuf[*DisplayData.currentPulseRateIndex];
       if(*WarningAlarmData.pulseOutOfRange && ((currentPulseCorrected > 115) || (currentPulseCorrected < 51))) {
        if((annonciationCounter > 4)) {
          Serial.println("Alarm Active");
        } else {
          Serial.println("Alarm Active (Acknowledged)");
        }
    } else if(*WarningAlarmData.pulseOutOfRange) {
      if (((currentPulseCorrected > 105) && (currentPulseCorrected < 115)) || ((currentPulseCorrected < 57) && (currentPulseCorrected > 51))) {
          Serial.println("Warning (Blinking)");
      } else {
        Serial.println("Warning");
      }
    } else {
      Serial.println("Normal Range");
    } 
      
      Serial.print("* Resp.:   ");
       //decides respirations current alarm state
      int currentRespCorrected = (int)DisplayData.respirationCorrectedBuf[*DisplayData.currentRespirationIndex];
       if(*WarningAlarmData.respOutOfRange && ((currentRespCorrected > 28) || (currentRespCorrected < 10))) {
        if((annonciationCounter > 4)) {
          Serial.println("Alarm Active");
        } else {
          Serial.println("Alarm Active (Acknowledged)");
        }
    } else if(*WarningAlarmData.respOutOfRange) {
        Serial.println("Warning");
    } else {
      Serial.println("Normal Range");
    }
      
      Serial.print("* Battery: "); 
       //decides batteries current alarm state
      if(*WarningAlarmData.batteryOutOfRange) {
        if((annonciationCounter > 4)){
          Serial.println("Alarm Active");
        } else {
          Serial.println("Alarm Active (Acknowledged)");
        }
     } else {
       Serial.println("Normal Range");
     }
    }
    else {
      // Unknown Command
      Serial.println("E: Command Not Recognized");
    }
  }
}

/*
 * Sends a message to the local device(Uno), with format:
 * 1. Start of message
 * 2. Requested task identifier
 * 3. ID of function
 * 4. Requested data
 * 5. End of message
 */
void sendLocalMessage(byte startByte,
                      byte task,
                      byte ID,
                      byte data,
                      byte endByte) {
  Serial1.write(startByte);
  Serial1.write(task);
  Serial1.write(ID);
  Serial1.write(data);
  Serial1.write(endByte);
}

/*
 * Converts 'input' into a uppercase letter
 * if it is a lowercase letter
 * (Returns same char if not a lowercase char)
 */
byte toUpper(byte input) {
  if(input >= 0x61 && input <= 0x7A) {
    return input - 0x20;
  }
}

/*
 * Waits for either S or P response
 * Returns true  if 'S'
 * Returns false if 'P'
 */
bool waitResponseSP() {
  while(true) {
    if(Serial.available() > 0) {
      byte response = toUpper(Serial.read());
      if(response == S) {
        return true;
      }
      else if(response == P) {
        return false;
      }
      else {
        Serial.println("E: You must enter either 'S' or 'P'");
      }
    }
  }
}
