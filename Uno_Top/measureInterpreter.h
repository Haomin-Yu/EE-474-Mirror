// This is simply used to work as a bridge between the C file and 
// arduino code so we can use it with our arduino code.

#ifndef __MEASUREINTERPRETER_H__
#define __MEASUREINTERPRETER_H__

#ifdef __cplusplus
extern "C" {
#endif

void pulseRateInterpreter(unsigned int* prValuePointer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __MEASUREINTERPRETER_H__ */
