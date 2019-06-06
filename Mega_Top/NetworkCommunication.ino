/* 
 * This class handles the communication with the Arduino Uno
 * and the remote device
 * ==============================================================
 * getTemp : E700FFD8
 * getBP   : E701FFD8
 * getRR   : E702FFD8
 * getPulse: E703FFD8
 * getEKG  : E704FFD8
 * 
 * Author: Haomin Yu
 */
#include "compute.h"

// Function prototypes
void remoteCommunication();
void sendLocalMessage(byte startByte, byte task, byte ID, byte data, byte endByte);
void sendRemoteMessage(byte startByte, byte task, byte ID, int data, byte endByte);

// Network Identifiers
const static byte START = 0xE7;
const static byte   END = 0xDB;
const static byte    NA = 0xFF;

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
void remoteCommunication() {
  if(Serial.available() > 0) {
    // Waiting for all bytes to come in
    delay(10);
    if(Serial.available() == 8) {
      // Throwing away start byte
      Serial.read();
      Serial.read();
      // Grabbing task byte
      byte task = convertHexDump(Serial.read()) << 4;
      task |= convertHexDump(Serial.read());
      // Throwing away function request
      Serial.read();
      Serial.read();
      // Throwing away end byte
      Serial.read();
      Serial.read();
      // Executing task and sending message
      unsigned int measuredData = 0;
      double* arrayPointer = 0;
      switch(task) {
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
          measuredData = (unsigned int)illusionGenerator(computeEKG(arrayPointer));
          break;
        default:
          remoteDataMessage = "Unknown Function Type!";
          measuredData = 0;
          annonciationCounter++;
          break;
      }
      annonciationCounter--;
      sendRemoteMessage(START, task, NA, measuredData, END);
    }
    else { // Flush
      Serial.println("Invalid input has been flushed!");
      delay(5);
      while(Serial.available() > 0) {
        Serial.read();
      }
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
 * Converts string hex dump to its hex respresentation
 */
byte convertHexDump(byte hexDump) {
  if(hexDump >= 0x30 && hexDump <= 0x39) {
    return hexDump - 0x30;
  }
  else if(hexDump >= 0x41 && hexDump <= 0x46) {
    return hexDump - 0x37;
  }
  else if(hexDump >= 0x61 && hexDump <= 0x66) {
    return hexDump - 0x57;
  }
  else {
    return 0xFF;
  }
}
