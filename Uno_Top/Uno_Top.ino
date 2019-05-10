/*
 * Function name: Project_2_Uno
 * Function inputs: takes in the byte interpreter, as well as information from the mega
 * Function outputs: outputs data from the measure sensors or simulator and sends them to the mega.
 * Function description: this is mainly used to read serial information from the mega and send serial information to the mega.
 * Author: Haomin Yu
 */
extern "C" {                                            //calls in files
  #include "byteInterpreter.h"
  #include "measureInterpreter.h"
}

void setup() {                                          //sets up the serial for sending and recieving information.
  // Setting the baud rate
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {                          //if a serial is available it will recieve incoming information and send out the measured data
     unsigned char incoming = Serial.read();
     unsigned int    output = interpretByte(incoming);
     Serial.write(output);
  }
}
