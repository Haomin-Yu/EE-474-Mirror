/* 
 * This class handles the communication with the Arduino Uno
 * and the remote device
 * ==============================================================
 * 
 * Author: Haomin Yu
 */
#include "compute.h"

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
        tempCheck = true;
        Serial.print("Remotely Received Temperature = "); Serial.println((int)computeTemp(getSerialTemp()));
      }
      else {
        Serial.println("P: Measurement has stopped");
        goto stopMeasurement;
      }
      // TODO - See lines 134-167
      // Getting Blood Pressure
      // Getting Resp. Rate
      // Getting Pulse Rate
      // Getting EKG
      stopMeasurement:;
    }
    else if(command == P) {
      // Break from measurement
      Serial.println("E: No measurement is ongoing. Nothing to stop.");
    }
    else if(command == D) {
      // Toggles TFT display
      if(displayOn) {
        // Makes the whole screen black
        tft.fillScreen(BLACK);
      }
      else {
        // Repainting labels, measurements, and buttons
        // TODO
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
      Serial.print("* Temperature  = "); Serial.print(temp); Serial.println("\t C");
      Serial.print("*  Sys. Press. = "); Serial.print(sys);  Serial.println("\t mmHg");
      Serial.print("* Dias. Press. = "); Serial.print(dias); Serial.println("\t mmHg");
      Serial.print("* Pulse Rate   = "); Serial.print(pr);   Serial.println("\t BPM");
      Serial.print("* Resp. Rate   = "); Serial.print(resp); Serial.println("\t BPM");
      Serial.print("* EKG          = "); Serial.print(ekg);  Serial.println("\t Hz");
      Serial.print("* Battery      = "); Serial.print(batt); Serial.println();
    }
    else if(command == W) {
      // Prints out information from 'WarningAlarmData'
      Serial.println("W: Printing out most recent warning/alarm information:");
      Serial.print("* Temp    out of range? "); Serial.println(*WarningAlarmData.tempOutOfRange);
      Serial.print("* BP      out of range? "); Serial.println(*WarningAlarmData.bpOutOfRange);
      Serial.print("* Pulse   out of range? "); Serial.println(*WarningAlarmData.pulseOutOfRange);
      Serial.print("* Resp.   out of range? "); Serial.println(*WarningAlarmData.respOutOfRange);
      Serial.print("* Battery out of range? "); Serial.println(*WarningAlarmData.batteryOutOfRange);
      Serial.print("* Temp. High? "); Serial.println(*WarningAlarmData.tempHigh);
      Serial.print("* BP.   High? "); Serial.println(*WarningAlarmData.bpHigh);
      Serial.print("* Resp. High? "); Serial.println(*WarningAlarmData.respHigh);
      Serial.print("* Pulse   Low? "); Serial.println(*WarningAlarmData.pulseLow);
      Serial.print("* Battery Low? "); Serial.println(*WarningAlarmData.batteryLow);
    }
    else {
      // Unknown Command
      Serial.println("E: Command Not Recognized");
    }
  }
  
     /*
        case measureTemperatureFunc:
          tempCheck = true;
          remoteDataMessage = "Remotely Received Temperature = ";
          measuredData = computeTemp(getSerialTemp());
          break;
        case measureBloodPressureFunc:
          bloodPressCheck = true;
          remoteDataMessage = "Remotely Received BloodPressure = ";
          measuredData = getBloodPress();
          measuredData = (int)computeSys((unsigned int)measuredData >> 8) << 8 
                       | (int)computeDias(measuredData & 0xFF);
          break;
        case measureRespirationFunc:
          respirationCheck = true;
          remoteDataMessage = "Remotely Received Respiration = ";
          measuredData = computeRespiration(getRespiration());
          break;
        case measurePulseRateFunc:
          pulseCheck = true;
          remoteDataMessage = "Remotely Received Pulse Rate = ";
          measuredData = computePr(getPulseRate());
          break;
        case measureEKGFunc:
          ekgCheck = true;
          remoteDataMessage = "Remotely Received Peak EKG = ";
          arrayPointer = getEKG();
          measuredData = (unsigned int)computeEKG(arrayPointer);
          break;
        default:
          remoteDataMessage = "Unknown Function Type!";
          measuredData = 0;
          annonciationCounter++;
          break;
      */
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
