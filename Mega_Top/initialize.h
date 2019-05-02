//this file is simply used as a bride between the c code and arduino code so it may be easily called.
#ifndef __INITIALIZE_H__
#define __INITIALIZE_H__

#ifdef __cplusplus
extern "C" {
#endif

void initialize(MeasureDataStruct* MeasureData,
                DisplayDataStruct* DisplayData,
                ComputeDataStruct* ComputeData,
                WarningAlarmDataStruct* WarningAlarmData,
                StatusStruct*    Status,
                SchedulerStruct* Scheduler);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __INITIALIZE_H__ */
