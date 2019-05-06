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
        data.temperatureRawBuf[*data.nextTemperatureIndex] = getSerialTemp();
        *data.nextTemperatureIndex = (*data.nextTemperatureIndex + 1) % 8;
        break;
      case measureSysPress:
        data.bloodPressRawBuf[*data.nextSysPressIndex] = getSysPress();
        *data.nextSysPressIndex = (*data.nextSysPressIndex + 1) % 8;
        break;
      case measureDiasPress:
        data.bloodPressRawBuf[*data.nextDiasPressIndex] = getDiasPress();
        *data.nextDiasPressIndex = ((*data.nextDiasPressIndex + 1) % 8) + 8;
        break;
      case measurePulseRate:
        data.pulseRateRawBuf[*data.nextPulseRateIndex] = getPulseRate();
        *data.nextPulseRateIndex = (*data.nextPulseRateIndex + 1) % 8;
        break;
      default:
        break;
    }
}
