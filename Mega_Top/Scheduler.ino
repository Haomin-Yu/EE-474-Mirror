/*
 * Function name: Scheduler
 * Function inputs: compute.h file, status.h file, display.h file, rawStructs.h file
 * Function outputs: there arent really any outputs. Its mainly just ran and executes functions 
 *                   in the order this function calls them.
 * Function description: this creates a task queue and schedules the execution of the tasks for
 *                       the system based off of the order the tasks are assigned and a internal
 *                       clock stating when the tasks are actually meant to be executed.
 * Author: Nathan Ness
 * 
 */

 //importing files
extern "C" {
  #include "compute.h"
  #include "status.h"
  #include "display.h"
  #include "rawStructs.h"
}
// Getting global structs
extern MeasureDataStruct MeasureData;                                                             //takes information from measure datastruct and saves the data for use
extern DisplayDataStruct DisplayData;                                                             //takes information from dispay datastruct and saves the data for use
extern ComputeDataStruct ComputeData;                                                             //takes information from compute datastruct and saves the data for use
extern WarningAlarmDataStruct WarningAlarmData;                                                   //takes information from warning and alarm datastruct and saves the data for use
extern StatusStruct Status;                                                                       //takes information from status datastruct and saves the data for use

// Getting global indicators
extern bool enableMeasure;                                                                        //creates an enable variable to tell when measure should run
extern bool enableCompute;                                                                        //creates an enable variable to tell when compute should run
extern bool enableDisplay;                                                                        //creates an enable variable to tell when display should run
extern bool enableStatus;                                                                         //creates an enable variable to tell when status should run

TCB MeasurementTask = (TCB) {                                                                     //defines a task for measurements
  &measure,
  &MeasureData
};

TCB ComputationTask = (TCB) {                                                                     //defines a task for computation
  &compute,
  &ComputeData
};

TCB StatusTask = (TCB) {                                                                          //defines a task for status
  &status,
  &Status
};

TCB AlarmTask = (TCB) {                                                                           //defines a task for alarms and warnings
  &alarm,
  &WarningAlarmData
};

TCB DisplayTask = (TCB) {                                                                         //defines a task for display
  &display,
  &DisplayData
};

TCB taskQueue[] = {MeasurementTask, ComputationTask, StatusTask, AlarmTask, DisplayTask, NULL};   //creates an array of the tasks so they can easily be executed in order.

// Global constants
static const unsigned long EACH_TASK_TIME = 5000;                                                 //defines the constant that states how often the tasks other than alarm will
                                                                                                  //be executed. in this case its set to 5 seconds but can be changed in milliseconds.
void scheduler() {                                                                                //creates scheduler function that is to be called in a loop to constantly run the order.
  static unsigned long previousTime = 0;
  for(int i = 0; i < 5; i++) {
    taskQueue[i].myTask(taskQueue[i].taskDataPtr);                                                //loops through tasks to be executed.
  }
  if((millis() - previousTime) > EACH_TASK_TIME) {                                                //if its been 5 seconds or more sets enable true so the functions will execute.
    previousTime  = millis();
    enableMeasure = true;
    enableCompute = true;
    enableDisplay = true;
    enableStatus  = true;
  } else {                                                                                        //if it hasnt been 5 seconds or more sets enable to false so functions wont execute.
    enableMeasure = false;
    enableCompute = false;
    enableDisplay = false;
    enableStatus  = false;
  }
}
