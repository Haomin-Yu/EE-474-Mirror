  /*
 * Function name: Project_2_Uno
 * Function inputs: takes in the byte interpreter, as well as information from the mega
 * Function outputs: outputs data from the measure sensors or simulator and sends them to the mega.
 * Function description: this is mainly used to read serial information from the mega and send serial information to the mega.
 * 
 * Author: Haomin Yu
 */
// Compiler Linkage
extern "C" {
  #include "byteInterpreter.h"
  #include "measureInterpreter.h"
  #include "init.h"
  #include "arduinoFFT.h"
}

// Pin assignments
static const int       PULSE_INTERRUPT = 2;
static const int RESPIRATION_INTERRUPT = 3;
static const int    NETWORK_CORRUPTION = 4;
static const int       PULSE_DIGITAL_OUT = 12;
static const int RESPIRATION_DIGITAL_OUT = 13;
static const int         EKG_ANALOG_IN = A0;
static const int        TEMP_ANALOG_IN = A1;
static const int      BUTTON_ANALOG_IN = A2;
static const int      SWITCH_ANALOG_IN = A3;
static const int       PULSE_ANALOG_IN = A4;
static const int RESPIRATION_ANALOG_IN = A5;

// Grabbing external variables
extern unsigned int* bloodPressurePointer;

// Network Identifiers
const static byte START = 0xE7;
const static byte   END = 0xDB;
const static byte    NA = 0xFF;

// FFT object
arduinoFFT FFT = arduinoFFT();

void setup() {                                          //sets up the serial for sending and recieving information.
  // Setting the baud rate
  Serial.begin(9600);
  pinMode(PULSE_DIGITAL_OUT, OUTPUT);
  pinMode(RESPIRATION_DIGITAL_OUT, OUTPUT);
  pinMode(NETWORK_CORRUPTION, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {                          //if a serial is available it will recieve incoming information and send out the measured data
     // Allowing for all bytes to come in
     delay(5);
     // Decoding the message
     if(Serial.available() == 5) {
       // Throwing away start byte
       Serial.read();
       // Grabbing task byte
       byte task = Serial.read();
       // Throwing away function request
       Serial.read();
       // Throwing away data request
       Serial.read();
       // Throwing away end byte
       Serial.read();
       // Executing task and sending message
       if(task == measureBloodPressure) {
         *bloodPressurePointer = bloodPressureRaw_INIT;
         unsigned int data1 = interpretByte(task);
         unsigned int data2 = interpretByte(task);
         sendMessage(START, NA, NA, (byte)data1, (byte)data2, END);
       }
       else if(task == measureEKG) {
         
       }
       else {
         unsigned int data = interpretByte(task);
         sendMessage(START, NA, NA, (byte)data, END);
       }
     }
     else { // Flush
       while(!Serial.available() == 0) {
         Serial.read();
       }
     }
  }
}

/* Sends a message with the format:
 * 1. Start of message
 * 2. Requesting task identifier
 * 3. Function being requested
 * 4. Data being returned by the function
 * 5. End of message
 */
void sendMessage(byte startByte,
                 byte identifier, 
                 byte task,
                 byte data, 
                 byte endByte) {
  Serial.write(startByte);
  Serial.write(identifier);
  Serial.write(task);
  Serial.write(data);
  Serial.write(endByte);
}


/* Sends a message with the format:
 * 1. Start of message
 * 2. Requesting task identifier
 * 3. Function being requested
 * 4. Data1 being returned by the function
 * 5. Data2 being returned by the function
 * 6. End of message
 */
void sendMessage(byte startByte,
                 byte identifier, 
                 byte task,
                 byte data1,
                 byte data2, 
                 byte endByte) {
  Serial.write(startByte);
  Serial.write(identifier);
  Serial.write(task);
  Serial.write(data1);
  Serial.write(data2);
  Serial.write(endByte);
}
