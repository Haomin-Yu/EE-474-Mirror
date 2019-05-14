/*
 * Function name: Alarms and Warnings
 * Function inputs: WarningAlarmDataStruct containing the raw data
 *                  from measure. As well as the inital data.
 * Function outputs: Booleans for each measurement indicating if the system
 *                   is in warning or alarm.
 * Function description: This takes the raw data from sensors and checks to see if
 *                       any need a warning or alarm. If they do the function
 *                       returns a value of true to its corresponding warning
 *                       and returns a 1 to its corresponding alarm if they should be
 *                       triggered.
 * Author: Nathan Ness
 * 
 * 
 */
// Compiler Linkage
extern "C" {
  #include "rawStructs.h"
  #include "init.h"
}

//imports tft struct
extern TFTKeypadDataStruct KeypadData;

// Function prototypes
void alarm(void* Data);                                                                        //assigns the alarm function
unsigned char bpRangeAlarm(unsigned int systolicPressRaw, unsigned int diastolicPressRaw);     //assigns an alarm for blood pressure
unsigned char tempRangeAlarm(unsigned int temperatureRaw);                                     //assigns an alarm for temperature
unsigned char pulseRangeAlarm(unsigned int pulseRateRaw);                                      //assigns an alarm for pulse
unsigned char batteryRangeAlarm(unsigned int batteryState);                                    //assigns an alarm for battery
bool bpRangeWarning(unsigned int systolicPressRaw, unsigned int diastolicPressRaw);            //assigns a warning for blood pressure
bool tempRangeWarning(unsigned int temperatureRaw);                                            //assigns a warning for temperature
bool pulseRangeWarning(unsigned int pulseRateRaw);                                             //assigns a warning for pulse
bool batteryRangeWarning(unsigned int batteryState);                                           //assigns a warning for battery

void alarm(void* Data) {
  WarningAlarmDataStruct data = *((WarningAlarmDataStruct*)Data);                              //sets the datastructure to contatin the information needed
  unsigned short tempIndex  = *data.currentTemperatureIndex;
  unsigned short sysIndex   = *data.currentSysPressIndex;
  unsigned short diasIndex  = *data.currentDiasPressIndex;
  unsigned short pulseIndex = *data.currentPulseRateIndex;
  
  *data.bpHigh     = bpRangeWarning(data.bloodPressRawBuf[sysIndex], data.bloodPressRawBuf[diasIndex]);      //calls the function to test for the blood pressure warning
  *data.tempHigh   = tempRangeWarning(data.temperatureRawBuf[tempIndex]);                                    //calls the function to test for the temperature warning
  *data.pulseLow   = pulseRangeWarning(data.pulseRateRawBuf[pulseIndex]);                                    //calls the function to test for the pulse warning
  *data.batteryLow = batteryRangeWarning(*data.batteryState);                                                //calls the function to test for the battery warning
  *data.bpOutOfRange      = bpRangeAlarm(data.bloodPressRawBuf[sysIndex], data.bloodPressRawBuf[diasIndex]); //calls the function to test for the blood pressure alarm
  *data.tempOutOfRange    = tempRangeAlarm(data.temperatureRawBuf[tempIndex]);                               //calls the function to test for the temperature alarm
  *data.pulseOutOfRange   = pulseRangeAlarm(data.pulseRateRawBuf[pulseIndex]);                               //calls the function to test for the pulse alarm
  *data.batteryOutOfRange = batteryRangeAlarm(*data.batteryState);                                           //calls the function to test for the battery alarm
}

unsigned char bpRangeAlarm(unsigned int systolicPressRaw, unsigned int diastolicPressRaw) {  //function runs if checks to see if the alarm should be on
  if (((systolicPressRaw < 51) || (systolicPressRaw > 56)) ||                                //for blood pressure. If it is it returns 1, if not it returns 0
     ((diastolicPressRaw > 47) || (diastolicPressRaw < 41))) {                               //also it will decide if the alarmacknowledgement must be changed
      if(alarmCheck && (annonciationCounter > 4)) {
        *KeypadData.alarmAcknowledge = 1;
      }
      return 1;
  } 
  return 0;
}

unsigned char tempRangeAlarm(unsigned int temperatureRaw) {                                    //function runs if checks to see if the alarm should be on
  if ((temperatureRaw < 42) || (temperatureRaw > 44)) {                                        //for temperature. If it is it returns 1, if not it returns 0
    if(alarmCheck && (annonciationCounter > 4)) {                                              //also it will decide if the alarmacknowledgement must be changed
      *KeypadData.alarmAcknowledge = 1;
    }
    return 1;
  }
  return 0;
}

unsigned char pulseRangeAlarm(unsigned int pulseRateRaw) {                                     //function runs if checks to see if the alarm should be on
  if((pulseRateRaw < 60) || (pulseRateRaw > 100)) {                                            //for pulse. If it is it returns 1, if not it returns 0
    if(alarmCheck && (annonciationCounter > 4)) {                                              //also it will decide if the alarmacknowledgement must be changed
      *KeypadData.alarmAcknowledge = 1;
    }
    return 1;
  }
  return 0;
}

unsigned char batteryRangeAlarm(unsigned short batteryState) {                                 //function runs if checks to see if the alarm should be on
  if(batteryState < 40) {                                                                      //for batery. If it is it returns 1, if not it returns 0
    if(alarmCheck && (annonciationCounter > 4)) {                                              //also it will decide if the alarmacknowledgement must be changed
      *KeypadData.alarmAcknowledge = 1;
    }
    return 1;
  }
  return 0;
}

bool bpRangeWarning(unsigned int systolicPressRaw, unsigned int diastolicPressRaw) {          //function runs if checks to see if the warning should be on
  if ((systolicPressRaw > 56) || (diastolicPressRaw > 50)) {                                  //for blood pressure. If it is it returns true, if not it returns false.
      if(alarmCheck && (annonciationCounter > 4)) {                                           //also it will decide if the alarmacknowledgement must be changed
        *KeypadData.alarmAcknowledge = 1;
      }
      return true;
  } 
  return false;
}

bool tempRangeWarning(unsigned int temperatureRaw) {                                           //function runs if checks to see if the warning should be on
  if (temperatureRaw > 44) {                                                                   //for temperature. If it is it returns true, if not it returns false.
    if(alarmCheck && (annonciationCounter > 4)) {                                              //also it will decide if the alarmacknowledgement must be changed
      *KeypadData.alarmAcknowledge = 1;
    }
    return true;
  }
  return false;
}

bool pulseRangeWarning(unsigned int pulseRateRaw) {                                            //function runs if checks to see if the warning should be on
  if(pulseRateRaw < 17) {                                                                      //for pulse. If it is it returns true, if not it returns false.
    if(alarmCheck && (annonciationCounter > 4)) {                                              //also it will decide if the alarmacknowledgement must be changed
      *KeypadData.alarmAcknowledge = 1;
    }
    return true;
  }
  return false;
}

bool batteryRangeWarning(unsigned short batteryState) {                                        //function runs if checks to see if the warning should be on
  if(batteryState < 40) {                                                                      //for battery. If it is it returns true, if not it returns false.
    if(alarmCheck && (annonciationCounter > 4)) {                                              //also it will decide if the alarmacknowledgement must be changed
      *KeypadData.alarmAcknowledge = 1;
    }
    return true;
  }
  return false;
}
