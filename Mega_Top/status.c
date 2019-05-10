/*
 * Function name: status
 * Function inputs: this inputs the raw structs for the status data structure.
 * Function outputs: this will output the battery data to the data structure.
 * Function description: This serves to save the battery data to its data structure to be 
 *                       used throughout the rest of the project.
 * Author: Haomin Yu
  */
#include "rawStructs.h"

// Function Prototypes
void status(void* Data);

// Global variable
bool enableStatus = false;

// Status of the system
void status(void* Data) {
    if(enableStatus) {
      StatusStruct data = *((StatusStruct*)Data);
      if(*data.batteryState > 0) {
        *data.batteryState = *data.batteryState - 1;
      }
      enableStatus = false;
    }
}
