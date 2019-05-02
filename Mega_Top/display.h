//this file is simply used as a bride between the c code and arduino code so it may be easily called.
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

void display(void* Data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __DISPLAY_H__ */
