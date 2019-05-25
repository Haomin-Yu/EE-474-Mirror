/*
 * Function name: Scheduler
 * Function inputs: compute.h file, status.h file, display.h file, rawStructs.h file
 * Function outputs: there arent really any outputs. Its mainly just ran and executes functions 
 *                   in the order this function calls them.
 * Function description: this creates a task queue and schedules the execution of the tasks for
 *                       the system based off of the order the tasks are assigned and a internal
 *                       clock stating when the tasks are actually meant to be executed.
 * Author: Nathan Ness & Nathan Park
 * 
 */
// Importing files
extern "C" {
  #include "compute.h"
  #include "status.h"
  #include "rawStructs.h"
}
// Getting global structs
extern MeasureDataStruct MeasureData;                                                             //takes information from measure datastruct and saves the data for use
extern DisplayDataStruct DisplayData;                                                             //takes information from dispay datastruct and saves the data for use
extern ComputeDataStruct ComputeData;                                                             //takes information from compute datastruct and saves the data for use
extern WarningAlarmDataStruct WarningAlarmData;                                                   //takes information from warning and alarm datastruct and saves the data for use
extern StatusStruct Status;                                                                       //takes information from status datastruct and saves the data for use
extern CommunicationsDataStruct CommunicationsData;                                               //takes information from communcations datastruct and saves the data for use

TCB nullTCB = (TCB) {                                                                     //defines a task for measurements
  NULL,
  NULL,
  NULL,
  NULL
};

bool operator == (const TCB task1, const TCB task2) {
  if ((task1.myTask == task2.myTask) and (task1.taskDataPtr == task2.taskDataPtr)) {
    return true;
  }
  else {
    return false;
  }
}

bool operator != (const TCB task1, const TCB task2) {
  if ((task1.myTask != task2.myTask) or (task1.taskDataPtr != task2.taskDataPtr)) {
    return true;
  }
  else {
    return false;
  }
}

TCB* Head = NULL;
TCB* Tail = NULL;


TCB MeasurementTask = (TCB) {                                                                     //defines a task for measurements
  &measure,
  &MeasureData,
  NULL,
  NULL
};

TCB ComputationTask = (TCB) {                                                                     //defines a task for computation
  &compute,
  &ComputeData,
  NULL,
  NULL
};

TCB StatusTask = (TCB) {                                                                          //defines a task for status
  &status,
  &Status,
  NULL,
  NULL
};

TCB AlarmTask = (TCB) {                                                                           //defines a task for alarms and warnings
  &alarm,
  &WarningAlarmData,
  NULL,
  NULL
};

TCB DisplayTask = (TCB) {                                                                         //defines a task for display
  &display,
  &DisplayData,
  NULL,
  NULL
};

TCB CommunicateTask = (TCB) {                                                                     //defines a task for communication
  &remoteCommunication/* *myTask name */,
  &CommunicationsData,
  NULL,
  NULL
};

void insert(TCB* task) {                                        //allows for tasks to be inserted into the queue
  if(Head == NULL) {
    Head = task;
    Tail = task;
  } else {
    Tail->next = task;
    task->prev = Tail;
    Tail = task;
  }
}

void rmv(TCB* task) {                                             //allows for tasks to be removed from queue
  TCB *tempHead = Head;
  TCB *tempTail = Tail;
  if (tempHead != NULL and tempTail != NULL) {                    //checks to see if remove is redundant
    if (tempHead == tempTail) {                                   //checks for single node
      Head = NULL;
      Tail = NULL;
    } else if (*tempHead == *task) {                            //checks for head task to be removed
      tempHead->next->prev = NULL;
      Head = tempHead->next;
    } else if (*tempTail == *task) {                            //checks for last task to be removed
      tempTail->next->prev = NULL;
      Tail = tempTail->next;
    }
    else {
      //need to go through the linked list to remove
      TCB *temp = tempHead->next;
      while ((*temp != *task) and (*temp != nullTCB)) {
        temp = temp->next;
        tempHead = tempHead->next;
      }
      if (*temp != nullTCB) {
        tempHead->next = temp->next;
        if (temp->next != NULL) {
          temp->next->prev = tempHead->prev;
        }
      }
      
    }
  }
}

//TCB taskQueue[] = {MeasurementTask, ComputationTask, StatusTask, AlarmTask, DisplayTask, NULL};   //creates an array of the tasks so they can easily be executed in order.

// Global constants
const unsigned long BUTTON_TIME = 2000;                                                 //defines the constant that states how often the button should be checked
unsigned long previousTime = millis();                                                  //be executed. in this case its set to 2 seconds but can be changed in milliseconds.
void scheduler() {                                                                      //creates scheduler function that is to be called in a loop to constantly run the order.
  if(((millis() - previousTime) > BUTTON_TIME)) {
    previousTime = millis();
    if ((tempCheck || pulseCheck || bloodPressCheck)) {                                 //if any measure buttons were pressed they will be added to the task queue
      insert(&MeasurementTask);
      insert(&ComputationTask);
      insert(&StatusTask);
    }
  }
  insert(&AlarmTask);
  insert(&DisplayTask);
  insert(&CommunicateTask);
  while (Head != NULL) {                                                                          //loop through tasks
    Head->myTask(Head->taskDataPtr);
    rmv(Head);
  }
}
