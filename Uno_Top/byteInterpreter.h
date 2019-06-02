// This is simply used to work as a bridge between the C file and 
// arduino code so we can use it with our arduino code.

#ifndef __BYTEINTERPRETER_H__
#define __BYTEINTERPRETER_H__

#ifdef __cplusplus
extern "C" {
#endif

unsigned int interpretByte(unsigned char input);

#ifdef __cplusplus
} // extern "C"
#endif

#define measureTemperature   0x00
#define measureBloodPressure 0x01
#define measureRespiration   0x02
#define measurePulseRate     0x03
#define measureEKG           0x04

#endif /* __BYTEINTERPRETER_H__ */
