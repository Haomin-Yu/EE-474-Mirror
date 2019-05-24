
/* 
 * This class handles the communication with the Arduino Uno
 * and the remote device
 * 
 * Author: Haomin Yu
 */
 
// Network Identifiers
const static byte START = 0xE7;
const static byte   END = 0xDB;
const static byte    NA = 0xFF;

// Measure Definitions
const static byte measureTemperatureFunc   = 0x00;
const static byte measureBloodPressureFunc = 0x01;
const static byte measureRespirationFunc   = 0x02;
const static byte measurePulseRateFunc     = 0x03;

// Class variables
static String remoteDataMessage = "";

/*
 * Handles the remote communication from and to the system
 */
void remoteCommunication() {
  if(Serial.available() > 0) {
    // Waiting for all bytes to come in
    delay(5);
    if(Serial.available() == 4) {
      // Throwing away start byte
      Serial.read();
      // Grabbing task byte
      byte task = Serial.read();
      // Throwing away function request
      Serial.read();
      // Throwing away end byte
      Serial.read();
      // Executing task and sending message
      unsigned int measuredData  = 0;
      unsigned int measuredData2 = 0;
      switch(task) {
        case measureTemperatureFunc:
          remoteDataMessage = "Temperature = ";
          measuredData = getSerialTemp();
          break;
        case measureBloodPressureFunc:
          remoteDataMessage = "BloodPressure = ";
          getSysPress();
          getDiasPress();
          break;
        case measureRespirationFunc:
          remoteDataMessage = "Respiration = ";
          measuredData = getRespiration();
          break;
        case measurePulseRateFunc:
          remoteDataMessage = "Pulse Rate = ";
          measuredData = getPulseRate();
          break;
        default:
          measuredData = 0;
          break;
      }
      sendRemoteMessage(START, task, NA, measuredData, END);
    }
    else { // Flush
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
  Serial1.print(startByte, HEX);
  Serial1.print(task, HEX);
  Serial1.print(ID, HEX);
  Serial1.print(data, HEX);
  Serial1.print(endByte, HEX);
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
    Serial.print(data >> 8, DEC);
    Serial.print("//");
    Serial.print(data & 0xFF, DEC);
  }
  else {
    Serial.print(data, DEC);
  }
  Serial.println();
}
