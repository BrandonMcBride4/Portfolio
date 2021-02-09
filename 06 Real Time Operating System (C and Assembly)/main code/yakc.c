#include "clib.h"
#include "yakk.h"
#include "yaku.h"

#define NULL 0
#define FALSE 0
#define TRUE 1

#define IDLE_STACKSIZE 256

// YKCtxSwCount - Global variable tracking context switches, does this include switching from idle?
unsigned int YKCtxSwCount;

// YKIdleCount - Global variable used by idle task
unsigned int YKIdleCount;

// Global variable incremented by the kernel's tick handler
unsigned int YKTickNum;

int isrCallDepth;

// Saves the stack pointer in the isr
void asmSaveSP(void);

int **saveSP;
int *restoreSP;

int isYKRunning;
int currentTask;
int taskCount;
int semCount;
int qCount;
int eventCount;

struct tcb
{
    int *sp;
    int priority;
    int eventWaitMode;
    unsigned eventMask;
};

unsigned int tickCountDelays[MAXIMUM_NUMBER_OF_TASKS + 1];
int readyTasks[MAXIMUM_NUMBER_OF_TASKS + 1];
int blockedTasks[MAXIMUM_NUMBER_OF_TASKS + 1];
struct tcb tcbArray[MAXIMUM_NUMBER_OF_TASKS + 1];
YKSEM semArray[MAXIMUM_NUMBER_OF_SEMAPHORES];
YKQ qArray[MAXIMUM_NUMBER_OF_QUEUES];
YKEVENT eventArray[MAXIMUM_NUMBER_OF_EVENTS];

int IdleStk[IDLE_STACKSIZE];


void blockTask(int taskPriority)
{
//    printString("blockTask(");
//    printInt(taskPriority);
//    printString(")\n");
    readyTasks[taskPriority] = FALSE;
    blockedTasks[taskPriority] = TRUE;
}

void unblockTask(int taskPriority)
{
    readyTasks[taskPriority] = TRUE;
    blockedTasks[taskPriority] = FALSE;
}


// YKInitialize - Initializes all required kernel data structures
// Creates YKIdleTask
void YKInitialize(void)
{
//    printString("YKInitialize\n");
    YKCtxSwCount = 0;
    YKIdleCount = 0;
    YKTickNum = 0;
    isYKRunning = 0;
    isrCallDepth = 0;
    isYKRunning = 0;
    taskCount = 0;
    semCount = 0;
    qCount = 0;
    eventCount = 0;
    saveSP = NULL;
    restoreSP = NULL;
    currentTask = 0;

    YKNewTask(&YKIdleTask, &IdleStk[IDLE_STACKSIZE], 100);
}

void YKIdleTask(void)
{
    while (1)
    {
        YKEnterMutex();
        YKIdleCount++;
        YKIdleCount--;
        YKIdleCount++;
        YKExitMutex();
    }
}

void shiftRight(int oldI, int newI)
{
    if (newI > oldI)
    {
        blockedTasks[newI] = blockedTasks[oldI];
        readyTasks[newI] = readyTasks[oldI];
        tickCountDelays[newI] = tickCountDelays[oldI];
        tcbArray[newI] = tcbArray[oldI];
    }
}

// YKNewTask - Creates a new task
void YKNewTask(void (*task)(void), int *taskStack, unsigned char priority)
{
    int i;
//    printString("YKNewTask\n");
    YKEnterMutex();
    for (i = 1; i <= 12; ++i)
    {
        // Initialize stack
        switch (i)
        {
            case 1: // Flags
                *(taskStack - i) = 0x200;
                break;
            case 3: // IP
                *(taskStack - i) = (int) task;
                break;
            case 4: // BP
                *(taskStack - i) = (int) (taskStack - 1);
                break;
            default:
                *(taskStack - i) = 0;
                break;
        }
    }

    if (taskCount > 0)
    {
        i = taskCount - 1;
        while (tcbArray[i].priority > priority)
        {
            shiftRight(i, i + 1);
            i--;
            if (i == -1)
                break;
        }
        readyTasks[i + 1] = TRUE;
        blockedTasks[i + 1] = FALSE;
        tcbArray[i + 1].sp = taskStack - 12;
        tcbArray[i + 1].priority = priority;
        tickCountDelays[i + 1] = 0;
    }
    else
    {
        readyTasks[taskCount] = TRUE;
        blockedTasks[taskCount] = FALSE;
        tcbArray[taskCount].sp = taskStack - 12;
        tcbArray[taskCount].priority = priority;
        tickCountDelays[taskCount] = 0;
    }


    taskCount++;
    if (isYKRunning)
    {
        YKScheduler(TRUE);
        YKExitMutex();
    }
}

// YKRun - Starts actual execution of user code
void YKRun(void)
{
    int i;
//    printString("YKRun\n");
    isYKRunning = TRUE;
    YKCtxSwCount++;

    for (i = 0; i <= MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        if (readyTasks[i])
        {
            currentTask = i;

            restoreSP = tcbArray[i].sp;

//            printString("YKDispatcher, dispatching task ");
//            printInt(i);
//            printNewLine();
            YKDispatcher(FALSE);
        }
    }
}

// YKScheduler - Determines the highest priority ready task
void YKScheduler(int saveContextFlag)
{
    int i;
//    printString("YKScheduler\n");
    for (i = 0; i <= MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        if (readyTasks[i])
        {
            if (currentTask != i)
            {
//                printString("YKDispatcher, dispatching task ");
//                printInt(i);
//                printString(" from task ");
//                printInt(currentTask);
//                printNewLine();
                YKCtxSwCount++;

                saveSP = &(tcbArray[currentTask].sp);
                currentTask = i;

                restoreSP = tcbArray[i].sp;


                YKDispatcher(saveContextFlag);
                break;
            }
            else
            {
                break;
            }
        }
    }
}

void printSem(YKSEM *sem)
{
    int i;

    for (i = 0; i < MAXIMUM_NUMBER_OF_SEMAPHORES; ++i)
    {
        if (&(semArray[i]) == sem)
        {
            break;
        }
    }

    printString("\nSem Id: ");
    printInt(i);
    printNewLine();
    printString("Sem Value: ");
    printInt(sem->value);
    printNewLine();
    for (i = 0; i < MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        if (sem->blockedTasks[i])
        {
            printString("Task ");
            printInt(i);
            printString(" is blocked by this semaphore\n");
        }
    }
    printNewLine();
}

//Create and initialize a semaphore
YKSEM *YKSemCreate(int initialValue)
{
    int i;
    YKSEM *semReturn;

    YKEnterMutex();
    if (initialValue < 0)
    {
        return NULL;
    }

    semReturn = &(semArray[semCount]);
    semArray[semCount].value = initialValue;
    for (i = 0; i < MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        semArray[semCount].blockedTasks[i] = FALSE;
    }

    semCount++;

    if (isYKRunning)
    {
        YKExitMutex();
    }
    return semReturn;
}

//Obtain a semaphore
void YKSemPend(YKSEM *semaphore)
{
    YKEnterMutex();
    if (semaphore->value <= 0)
    {
        (semaphore->value)--;
        semaphore->blockedTasks[currentTask] = TRUE;
        readyTasks[currentTask] = FALSE;
        YKScheduler(TRUE);
    }
    else
    {
        (semaphore->value)--;
    }
    YKExitMutex();
}

//Release a semaphore
void YKSemPost(YKSEM *semaphore)
{
    int i;

    YKEnterMutex();
    (semaphore->value)++;
    for (i = 0; i < MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        if (semaphore->blockedTasks[i])
        {
            semaphore->blockedTasks[i] = FALSE;
            readyTasks[i] = TRUE;
            if (isrCallDepth == 0)
            {
                YKScheduler(TRUE);
            }
            break;
        }
    }
    YKExitMutex();
}

// Create and initialize a message queue
YKQ *YKQCreate(void **start, unsigned size)
{
    YKQ *q;

    // disable interrupts
    YKEnterMutex();

    // init q
    q = &qArray[qCount];
    qCount++;

    q->size = size;
    q->start = (int *) start;
    q->qEnd = q->start + size - 1;
    q->nextEmpty = q->start;
    q->oldestMsg = NULL;
    q->count = 0;

    // if isRunning enable interrupts
    if (isYKRunning)
    {
        YKExitMutex();
    }

    return q;
}

void YKQClear(YKQ *q)
{
    YKEnterISR(); //avoid inconsistancy

    q->nextEmpty = q->start;
    q->oldestMsg = NULL;
    q->count = 0;

    YKExitMutex();
}

// Obtain the oldest message from a queue
void *YKQPend(YKQ *queue)
{
    void *retMsg;

    // disable interrupts
    YKEnterMutex();

    // if oldest msg is null block task
    if (queue->count <= 0)
    {
        // block task
        queue->blockedTasks[currentTask] = TRUE;
        readyTasks[currentTask] = FALSE;
        YKScheduler(TRUE);
    }

    // assign oldest msg to return
    retMsg = (void *) *(queue->oldestMsg);
    (queue->oldestMsg)++; // inc oldest msg
    (queue->count)--; // dec count

    // if queue is empty oldest message is null
    if (queue->count <= 0)
    {
        queue->oldestMsg = NULL;
    }
        // else if necessary, wrap oldest msg around the q
    else if (queue->oldestMsg > queue->qEnd)
    {
        queue->oldestMsg = queue->start;
    }

    // enable interrupts
    YKExitMutex();
    return retMsg;
}

// Append a message to a queue
int YKQPost(YKQ *queue, void *msg)
{
    int i;
//    printString("posted q\n"); //!!!

    // disable interrupts
    YKEnterMutex();

    // if q is overflowing return 0
    if (queue->count > queue->size)
    {
        YKExitMutex();
        return 0;
    }

    // if q is empty update oldest msg to next empty
    if (queue->count <= 0)
    {
        queue->oldestMsg = queue->nextEmpty;
    }

    // append message to next empty
    *(queue->nextEmpty) = (int) (msg);
    (queue->count)++; // inc count
    (queue->nextEmpty)++; // inc next empty

    // if necessary, wrap next empty around the q
    if (queue->nextEmpty > queue->qEnd)
    {
        queue->nextEmpty = queue->start;
    }

    // unblock task that was blocked by the queue
    for (i = 0; i < MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
//        if(queue->blockedTasks[i]) {
//            printString("blocked ");
//
//        } else {
//            printString("ready ");
//        }
//        printInt(i);
//        printNewLine();
        if (queue->blockedTasks[i])
        {
            queue->blockedTasks[i] = FALSE;
            readyTasks[i] = TRUE;
            if (isrCallDepth == 0)
            {
                YKScheduler(TRUE);
            }
            break;
        }
    }

    // enable interrupts
    YKExitMutex();
    return 1;
}

// Create and initialize an event flag group
YKEVENT *YKEventCreate(unsigned initialValue)
{
    YKEVENT *newEvent;
    int i;

    YKEnterMutex();
    newEvent = &eventArray[eventCount];
    eventCount++;
    newEvent->value = initialValue;
    for (i = 0; i < MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        newEvent->blockedTasks[i] = FALSE;
    }

    if (isYKRunning)
    {
        YKExitMutex();
    }

    return newEvent;
}

// Cause calling task to block until a combination of event flag bits occurs
unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode)
{
    unsigned mask;

    YKEnterMutex();
    mask = eventMask & event->value;
    if (waitMode == EVENT_WAIT_ANY)
    {
        if (mask > 0) // if any events are set, return
        {
            YKExitMutex();
            return event->value;
        }
    }
    else // waitMode == EVENT_WAIT_ALL
    {
        if (mask == eventMask) // if all of the events are set, return
        {
            YKExitMutex();
            return event->value;
        }
    }

    // If events aren't set then block tasks
    tcbArray[currentTask].eventWaitMode = waitMode;
    tcbArray[currentTask].eventMask = eventMask;
    event->blockedTasks[currentTask] = TRUE;
    readyTasks[currentTask] = FALSE;

    YKScheduler(TRUE);

    YKExitMutex();
    return event->value;

}

// Set event flag bits (change to 1)
void YKEventSet(YKEVENT *event, unsigned eventMask)
{
    int i;

    YKEnterMutex();
    event->value = event->value | eventMask; // Set mask
    for (i = 0; i < MAXIMUM_NUMBER_OF_TASKS; ++i)
    {
        if (event->blockedTasks[i]) // Tasks only blocked by this event
        {
            if (tcbArray[i].eventWaitMode == EVENT_WAIT_ALL)
            {
                // If we need all then the event mask needs to match the event value
                if (tcbArray[i].eventMask != (tcbArray[i].eventMask & event->value))
                {
                    continue;
                }
            }
            else
            {
                // If any of them then aren't set don't unblock this task
                if ((tcbArray[i].eventMask & event->value) == 0)
                {
                    continue;
                }
            }

            // unblock task
            event->blockedTasks[i] = FALSE;
            readyTasks[i] = TRUE;
        }
    }

    if (isrCallDepth == 0)
    {
        YKScheduler(TRUE);
    }
    YKExitMutex();

}

// Clear event flag bits (change to 0)
void YKEventReset(YKEVENT *event, unsigned eventMask)
{
    YKEnterMutex();
    // makes the event 0 without changing other events in the group
    event->value = (~eventMask & event->value);
    YKExitMutex();
}

// YKEnterISR - Called on entry to ISR
void YKEnterISR(void)
{
//    printString("YKEnterISR\n");
    if (isrCallDepth == 0)
    {
        saveSP = &(tcbArray[currentTask].sp);
        asmSaveSP();
    }
    isrCallDepth++;
}

// YKExitISR - Called on exit from ISR
void YKExitISR(void)
{
//    printString("YKExitISR\n");
    isrCallDepth--;
    if (isrCallDepth == 0)
    {
        YKScheduler(FALSE);
    }

}

// YKDelayTask - Delays a task for specified number of clock ticks
void YKDelayTask(unsigned count)
{
    int tickCountEnd;

//    printString("YKDelayTask\n");
    if (count > 0)
    {
        // Delay
        YKEnterMutex();
        tickCountEnd = YKTickNum + count;
        tickCountDelays[currentTask] = tickCountEnd;
        blockTask(currentTask);
        YKScheduler(TRUE);
        YKExitMutex();
    }
}

// YKTickHandler - The kernel's timer tick interrupt handler
void YKTickHandler(void)
{
    int i;
//    printString("YKTickHandler\n");
    YKTickNum++;
//    printNewLine();
//    printString("TICK ");
//    printUInt(YKTickNum);
//    printNewLine();

    for (i = 0; i <= MAXIMUM_NUMBER_OF_TASKS; i++)
    {
//        printString("tickCountDelays[");
//        printInt(i);
//        printString("]: ");
//        printInt(tickCountDelays[i]);
//        printNewLine();
        if (YKTickNum >= tickCountDelays[i] && tcbArray[i].sp != NULL && blockedTasks[i])
        {
//            printString("unblock: ");
//            printInt(i);
//            printNewLine();
            unblockTask(i);
        }
    }
}
