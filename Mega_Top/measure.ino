/*
 * Function name: measure
 * Function inputs: takes in the measuredatastruct
 * Function outputs: sets the datastucts raw values to the measured data from sensors
 * Function description: this sets the raw data to the measured data from sensors so 
 *                       this data can be used in other files for portions such as warning and alarm
 *                       or computation
 * Author: Haomin Yu and Nathan Ness
 */ 
// Class constants
static const unsigned short outOfBounds      = 0;
static const unsigned short measureTemp      = 1;
static const unsigned short measureSysPress  = 2;
static const unsigned short measureDiasPress = 3;
static const unsigned short measurePulseRate = 4;
static const unsigned short measureRespiration = 5;
// Global variables
// (Indicating whether a new measurement has been made)
bool tempRawChanged      = true;
bool sysPressRawChanged  = true;
bool diasPressRawChanged = true;
bool pulseRateRawChanged = true;
bool respirationRawChanged = true;
// Global indicators
// (When the corrosponding button is pressed)
extern bool tempCheck;
extern bool sysCheck;
extern bool diasCheck;
extern bool pulseCheck;
extern bool respirationCheck;
extern bool enableStatus;
extern bool alarmCheck;

// Measures the data 'temperatureRaw', 'systolicPressRaw',
// 'diastolicPressRaw', and/or 'pulseRateRaw' beased on
// the given measureSelection in 'Data'
static const double THRESHOLD_PULSE_PERCENT = 15.0;
void measure(void* Data) {
  	MeasureDataStruct data = *((MeasureDataStruct*)Data);
    unsigned short select = *data.measurementSelection;
    unsigned short currentIndex;
    unsigned short nextIndex;
    unsigned int prevData;
    unsigned int incomingData;
    unsigned int dataDifference;
    switch(select) {
      case measureTemp:
        nextIndex = (*data.currentTemperatureIndex + 1) % 8;
        data.temperatureRawBuf[nextIndex] = getSerialTemp();
        *data.currentTemperatureIndex = nextIndex;
        tempRawChanged = true;
        tempCheck      = false;
        alarmCheck     = true;
        enableStatus   = true;
        tft.fillRect(10, 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, 12, 175, " Temp.");
        *data.measurementSelection = outOfBounds;
        break;
      case measureSysPress:
        nextIndex = (*data.currentSysPressIndex + 1) % 8;
        data.bloodPressRawBuf[nextIndex] = getSysPress();
        *data.currentSysPressIndex = nextIndex;
        sysPressRawChanged = true;
        sysCheck           = false;
        alarmCheck         = true;
        enableStatus       = true;
        tft.fillRect((12 + BUTTONWIDTH), 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, (14 + BUTTONWIDTH), 175, " Sys.");
        *data.measurementSelection = outOfBounds;
        break;
      case measureDiasPress:
        nextIndex = ((*data.currentDiasPressIndex + 1) % 8) + 8;
        data.bloodPressRawBuf[nextIndex] = getDiasPress();
        *data.currentDiasPressIndex = nextIndex;
        diasPressRawChanged = true;
        diasCheck           = false;
        alarmCheck          = true;
        enableStatus        = true;
        tft.fillRect((14 + BUTTONWIDTH * 2), 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " Dias.");
        *data.measurementSelection = outOfBounds;
        break;
      case measurePulseRate:
        currentIndex = *data.currentPulseRateIndex;
        nextIndex    = (*data.currentPulseRateIndex + 1) % 8;
         prevData     = data.pulseRateRawBuf[currentIndex];
         incomingData = getPulseRate();
        dataDifference = (incomingData > prevData)?
                                  (incomingData - prevData):
                                  (prevData - incomingData);
        if((dataDifference * 100.0 / prevData) > THRESHOLD_PULSE_PERCENT) {
          data.pulseRateRawBuf[nextIndex] = incomingData;
          *data.currentPulseRateIndex = nextIndex;
        }
        else {
          data.pulseRateRawBuf[currentIndex] = incomingData;
        }
        //changes check booleans accordingly as well as changes color of puse back to original color
        pulseRateRawChanged = true;
        pulseCheck          = false;
        alarmCheck          = true;
        enableStatus        = true;
        tft.fillRect((16 + BUTTONWIDTH * 3), 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, (18 + BUTTONWIDTH * 3), 175, "Pulse");
        *data.measurementSelection = outOfBounds;
        break;
      case measureRespiration:
        
        break;
      default:
        break;
    }
}
