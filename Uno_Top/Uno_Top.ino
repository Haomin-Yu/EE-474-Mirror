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
}

// Pin assignments
static const int       PULSE_INTERRUPT = 2;
static const int RESPIRATION_INTERRUPT = 3;
static const int       PULSE_DIGITAL_OUT = 12;
static const int RESPIRATION_DIGITAL_OUT = 13;
static const int        TEMP_ANALOG_IN = A1;
static const int         SYS_ANALOG_IN = A2;
static const int        DIAS_ANALOG_IN = A3;
static const int       PULSE_ANALOG_IN = A4;
static const int RESPIRATION_ANALOG_IN = A5;

void setup() {                                          //sets up the serial for sending and recieving information.
  // Setting the baud rate
  Serial.begin(9600);
  pinMode(PULSE_DIGITAL_OUT, OUTPUT);
  pinMode(RESPIRATION_DIGITAL_OUT, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {                          //if a serial is available it will recieve incoming information and send out the measured data
     unsigned char incoming = Serial.read();
     unsigned int    output = interpretByte(incoming);
     Serial.write(output);
  }
}
