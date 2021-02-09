#ifndef YAKK_H
#define YAKK_H

#include "yaku.h"

#define EVENT_WAIT_ANY 1
#define EVENT_WAIT_ALL 0

typedef struct
{
    int value;
    int blockedTasks[MAXIMUM_NUMBER_OF_TASKS];
} YKSEM;

typedef struct
{
    unsigned size;
    int count;
    int *start;
    int *qEnd;
    int *nextEmpty;
    int *oldestMsg;
    int blockedTasks[MAXIMUM_NUMBER_OF_TASKS];
} YKQ;

typedef struct
{
    unsigned value;
    int blockedTasks[MAXIMUM_NUMBER_OF_TASKS];

} YKEVENT;

// YKCtxSwCount - Global variable tracking context switches, does this include switching from idle?
extern unsigned int YKCtxSwCount;

// YKIdleCount - Global variable used by idle task
extern unsigned int YKIdleCount;

// Global variable incremented by the kernel's tick handler
extern unsigned int YKTickNum;

//Initializes all required kernel data structures
void YKInitialize(void);

//Disables interrupts
void YKEnterMutex(void);

//Enables interrupts
void YKExitMutex(void);

//Kernel's idle task
void YKIdleTask(void);

//Creates a new task
void YKNewTask(void (*task)(void), int *taskStack, unsigned char priority);

//Starts actual execution of user code
void YKRun(void);

//Determines the highest priority ready task
void YKScheduler(int saveContextFlag);

//Begins or resumes execution of the next task
void YKDispatcher(int saveContextFlag);

//Create and initialize a semaphore
YKSEM* YKSemCreate(int initialValue);

//Obtain a semaphore
void YKSemPend(YKSEM *semaphore);

//Release a semaphore
void YKSemPost(YKSEM *semaphore);

// Append a message to a queue
YKQ *YKQCreate(void **start, unsigned size);

// Obtain the oldest message from a queue
void *YKQPend(YKQ *queue);

// Create and initialize a message queue
int YKQPost(YKQ *queue, void *msg);

void YKQClear(YKQ* q);

// Create and initialize an event flag group
YKEVENT *YKEventCreate(unsigned initialValue);

// Cause calling task to block until a combination of event flag bits occurs
unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode);

// Set event flag bits (change to 1)
void YKEventSet(YKEVENT *event, unsigned eventMask);

// Clear event flag bits (change to 0)
void YKEventReset(YKEVENT *event, unsigned eventMask);

//Delays a task for specified number of clock ticks
void YKDelayTask(unsigned count);

//Called on entry to ISR
void YKEnterISR(void);

//Called on exit from ISR
void YKExitISR(void);

//The kernel's timer tick interrupt handler
void YKTickHandler(void);

#endif // YAKK_H
