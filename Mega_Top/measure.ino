/*
 * Function name: measure
 * Function inputs: takes in the measuredatastruct
 * Function outputs: sets the datastucts raw values to the measured data from sensors
 * Function description: this sets the raw data to the measured data from sensors so 
 *                       this data can be used in other files for portions such as warning and alarm
 *                       or computation
 * Author: Haomin Yu
 */ 
// Class constants
static const unsigned short measureTemp      = 0;
static const unsigned short measureSysPress  = 1;
static const unsigned short measureDiasPress = 2;
static const unsigned short measurePulseRate = 3;
// Measures the data 'temperatureRaw', 'systolicPressRaw',
// 'diastolicPressRaw', and/or 'pulseRateRaw' beased on
// the given measureSelection in 'Data'
void measure(void* Data) {
  	MeasureDataStruct data = *((MeasureDataStruct*)Data);
    unsigned short select = *data.measurementSelection;
    switch(select) {
      case measureTemp:
        *data.temperatureRaw    = getSerialTemp();
        break;
      case measureSysPress:
        *data.systolicPressRaw  = getSysPress();
        break;
      case measureDiasPress:
        *data.diastolicPressRaw = getDiasPress();
        break;
      case measurePulseRate:
        *data.pulseRateRaw      = getPulseRate();
        break;
      default:
        break;
    }
}
