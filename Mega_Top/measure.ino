/*
 * Function name: measure
 * Function inputs: takes in the measuredatastruct
 * Function outputs: sets the datastucts raw values to the measured data from sensors
 * Function description: this sets the raw data to the measured data from sensors so 
 *                       this data can be used in other files for portions such as warning and alarm
 *                       or computation
 * Author: Haomin Yu
 */
 
// Global variable
bool enableMeasure = true;

// Measures the data 'temperatureRaw', 'systolicPressRaw',
// 'diastolicPressRaw', and 'pulseRateRaw'
void measure(void* Data) {
	if(enableMeasure) {
  	MeasureDataStruct data = *((MeasureDataStruct*)Data);
    *data.temperatureRaw    = getSerialTemp();
    *data.systolicPressRaw  = getSysPress();
    *data.diastolicPressRaw = getDiasPress();
    *data.pulseRateRaw      = getPulseRate();
	}
}
