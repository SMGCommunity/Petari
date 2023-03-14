#include <revolution/os.h>

typedef struct OSShutdownFunctionQueue
{
    OSShutdownFunctionInfo* head;
    OSShutdownFunctionInfo* tail;
} OSShutdownFunctionQueue;

static OSShutdownFunctionQueue ShutdownFunctionQueue;

#define EnqueueTail(queue, info)                            \
do {                                                        \
    OSShutdownFunctionInfo* __prev;                         \
                                                            \
    __prev = (queue)->tail;                                 \
    if (__prev == NULL)                                     \
        (queue)->head = (info);                             \
    else                                                    \
        __prev->next = (info);                              \
    (info)->prev = __prev;                                  \
    (info)->next = NULL;                                    \
    (queue)->tail = (info);                                 \
} while (0)

#define EnqueuePrio(queue, info)                            \
do {                                                        \
    OSShutdownFunctionInfo* __prev;                         \
    OSShutdownFunctionInfo* __next;                         \
                                                            \
    for (__next = (queue)->head;                            \
         __next && __next->priority <= info->priority;      \
         __next = __next->next)                             \
            ;                                               \
    if (__next == NULL)                                     \
        EnqueueTail(queue, info);                           \
    else {                                                  \
        (info)->next = __next;                              \
        __prev = __next->prev;                              \
        __next->prev = (info);                              \
        (info)->prev = __prev;                              \
        if (__prev == NULL)                                 \
            (queue)->head = (info);                         \
        else                                                \
            __prev->next = (info);                          \
    }                                                       \
} while (0)

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info) {
    EnqueuePrio(&ShutdownFunctionQueue, info);
}

void OSResetSystem(int, u32, int) {
    OSPanic(__FILE__, 1130, "OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}