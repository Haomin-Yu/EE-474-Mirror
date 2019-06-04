// This is simply used to work as a bridge between the C file and 
// arduino code so we can use it with our arduino code.

#ifndef __MEASUREINTERPRETER_H__
#define __MEASUREINTERPRETER_H__

#ifdef __cplusplus
extern "C" {
#endif

void temperatureInterpreter(unsigned int* tempValuePointer);
void systolicPressInterpreter(unsigned int* sysValuePointer);
void diastolicPressInterpreter(unsigned int* diasValuePointer);
void pulseRateInterpreter(unsigned int* prValuePointer);
void respirationRateInterpreter(unsigned int* respirationValuePointer);
void ekgInterpreter(signed int* ekgValuePointer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __MEASUREINTERPRETER_H__ */
