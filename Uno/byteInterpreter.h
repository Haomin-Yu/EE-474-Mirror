//This is simply used to work as a bridge between the C file and arduino code so we can use it with our arduino code.
#ifndef __BYTEINTERPRETER_H__
#define __BYTEINTERPRETER_H__

#ifdef __cplusplus
extern "C" {
#endif

unsigned int interpretByte(unsigned char input);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __BYTEINTERPRETER_H__ */
