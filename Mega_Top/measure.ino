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
static const unsigned short outOfBounds        = 0;
static const unsigned short measureTemp        = 1;
static const unsigned short measureBloodPress  = 2;
static const unsigned short measureRespiration = 3;
static const unsigned short measurePulseRate   = 4;
static const unsigned short measureEKG         = 5;
// Global variables
// (Indicating whether a new measurement has been made)
bool tempRawChanged          = true;
bool bloodPressureRawChanged = true;
bool pulseRateRawChanged   = true;
bool respirationRawChanged = true;
bool ekgRawChanged         = true;
// Global indicators
// (When the corrosponding button is pressed)
extern bool tempCheck;
extern bool bloodPressCheck;
extern bool respirationCheck;
extern bool pulseCheck;
extern bool enableStatus;
extern bool alarmCheck;
extern bool ekgCheck;

// Measures the data 'temperatureRaw', 'systolicPressRaw',
// 'diastolicPressRaw', 'RespirationRaw', 'EKGRaw', and/or 'pulseRateRaw' beased on
// the given measureSelection in 'Data'

//precents deciding if it will be saved in the buffer
static const double THRESHOLD_PULSE_PERCENT       = 15.0;
static const double THRESHOLD_RESPIRATION_PERCENT = 15.0;
static const double THRESHOLD_TEMPERATURE_PERCENT = 15.0;
//actual function used to send what will be measured and take in the measured data
void measure(void* Data) {
  	MeasureDataStruct data = *((MeasureDataStruct*)Data);
    unsigned short select = *data.measurementSelection;
    unsigned short currentIndex;
    unsigned short nextIndex;
    unsigned short nextIndex2;
    unsigned int prevData;
    unsigned int incomingData;
    unsigned int incomingData2;
    unsigned int dataDifference;
    switch(select) {
      //selects temp measurement
      case measureTemp:
        currentIndex = *data.currentTemperatureIndex;
        nextIndex = (*data.currentTemperatureIndex + 1) % 8;
        prevData     = data.temperatureRawBuf[currentIndex];
        incomingData = getSerialTemp();
        dataDifference = (incomingData > prevData)?
                         (incomingData - prevData): (prevData - incomingData);
        if((dataDifference * 100.0 / prevData) > THRESHOLD_TEMPERATURE_PERCENT) {
          data.temperatureRawBuf[nextIndex] = incomingData;
          *data.currentTemperatureIndex = nextIndex;
        }
        else {
          data.temperatureRawBuf[currentIndex] = incomingData;
        }
        tempRawChanged = true;
        tempCheck      = false;
        alarmCheck     = true;
        enableStatus   = true;
        tft.fillRect(10, 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, 12, 175, " Temp.");
        *data.measurementSelection = outOfBounds;
        break;
        //selects blood pressure measurement
      case measureBloodPress:
        nextIndex  = (*data.currentSysPressIndex + 1) % 8;
        nextIndex2 = ((*data.currentDiasPressIndex + 1) % 8) + 8;
        incomingData = getBloodPress();
        incomingData2 = incomingData & 0xFF;
        incomingData  = (unsigned int)incomingData >> 8;
        data.bloodPressRawBuf[nextIndex]  = incomingData;
        data.bloodPressRawBuf[nextIndex2] = incomingData2;
        *data.currentSysPressIndex = nextIndex;
        *data.currentDiasPressIndex = nextIndex2;
        bloodPressureRawChanged = true;
        bloodPressCheck    = false;
        alarmCheck         = true;
        enableStatus       = true;
        tft.fillRect((12 + BUTTONWIDTH), 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, (14 + BUTTONWIDTH), 175, " B.P.");
        *data.measurementSelection = outOfBounds;
        break;
        //selects pulse measurement
      case measurePulseRate:
        currentIndex = *data.currentPulseRateIndex;
        nextIndex    = (*data.currentPulseRateIndex + 1) % 8;
        prevData     = data.pulseRateRawBuf[currentIndex];
        incomingData = getPulseRate();
        dataDifference = (incomingData > prevData)?
                         (incomingData - prevData): (prevData - incomingData);
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
        //selects respiration measurement
      case measureRespiration:
        currentIndex = *data.currentRespirationIndex;
        nextIndex = (*data.currentRespirationIndex + 1) % 8;
        prevData = data.respirationRateRawBuf[currentIndex];
        incomingData = getRespiration();
        dataDifference = (incomingData > prevData)?
                         (incomingData - prevData):
                         (prevData - incomingData);
        if((dataDifference * 100.0 / prevData) > THRESHOLD_RESPIRATION_PERCENT) {
          data.respirationRateRawBuf[nextIndex] = incomingData;
          *data.currentRespirationIndex = nextIndex;
        }
        else {
          data.respirationRateRawBuf[currentIndex] = incomingData;
        }
        respirationRawChanged = true;
        respirationCheck      = false;
        alarmCheck            = true;
        enableStatus          = true;
        tft.fillRect((14 + BUTTONWIDTH * 2), 160, BUTTONWIDTH, BUTTONHEIGHT, CYAN);
        TFT_Write(RED, (16 + BUTTONWIDTH * 2), 175, " R.R.");
        *data.measurementSelection = outOfBounds;
        break;
        //selects EKG measurement
      case measureEKG:
        getEKG();
        ekgRawChanged = true;
        ekgCheck      = false;
        alarmCheck    = true;
        enableStatus  = true;
        tft.fillRect(10, 202, (BUTTONWIDTH), (BUTTONHEIGHT), CYAN);             //changes color to represent a button press
          TFT_Write(RED, 12, 217," EKG");
        *data.measurementSelection = outOfBounds;
        break;
      default:
        break;
    }
}
