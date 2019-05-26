//this file is simply used as a bride between the c code and arduino code so it may be easily called.
#ifndef __COMPUTE_H__
#define __COMPUTE_H__

#ifdef __cplusplus
extern "C" {
#endif

void compute(void* Data);
double computeTemp(unsigned int tempRaw);
double computeSys(unsigned int systolicRaw);
double computeDias(unsigned int diastolicRaw);
double computePr(unsigned int bpRaw);
double computeRespiration(unsigned int respirationRaw);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __COMPUTE_H__ */
