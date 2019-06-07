/* 
 * This class handles the communication with the Arduino Uno
 * and the remote device
 * ==============================================================
 * getTemp : E700FFDB
 * getBP   : E701FFDB
 * getRR   : E702FFDB
 * getPulse: E703FFDB
 * getEKG  : E704FFDB
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
void remoteCommunication() {
  if(Serial.available() > 0) {
    byte command = toUpper(Serial.read());
    if(command == I) {
      if(initialized) {
        Serial.println("Re-initializing network..");
      }
      else {
        Serial.println("Initializing network..");
      }
      initialized = true;
      Serial.println("Done");
    }
    else if (!initialized){
      Serial.println("Network is not initialized!");
      Serial.println("Try using the command 'I'");
    }
    else if(command == E) {
      Serial.println("This an error response when incorrect or non-existent commands are given");
    }
    else if(command == S) {
      // Start Measurement
    }
    else if(command == P) {
      // Break from measurement
    }
    else if(command == D) {
      // Toggles TFT display
    }
    else if(command == M) {
      // Prints out information from 'DisplayData'
    }
    else if(command == W) {
      // Prints out information from 'WarningAlarmData'
    }
    else {
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
 * Sends a message to the remote device, with format:
 * 1. Start of message
 * 2. Requested task identifier
 * 3. ID of function
 * 4. Requested data
 * 5. End of message
 */
void sendRemoteMessage(byte startByte,
                       byte task,
                       byte ID,
                       int data,
                       byte endByte) {
  Serial.print(remoteDataMessage);
  if(task == measureBloodPressureFunc) {
    Serial.print((unsigned int)data >> 8, DEC);
    Serial.print("/");
    Serial.print(data & 0xFF, DEC);
  }
  else if(task == measureTemperatureFunc ||
          task == measurePulseRateFunc   ||
          task == measureRespirationFunc ||
          task == measureEKGFunc){
    Serial.print(data, DEC);
  }
  Serial.println();
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
