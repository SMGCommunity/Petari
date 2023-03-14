#include <revolution/os.h>

static void DefaultSwitchThreadCallback(OSThread *, OSThread *);

static volatile u32 RunQueueBits;
static OSThreadQueue RunQueue[32];
static volatile BOOL RunQueueHint;

static volatile s32 Reschedule;
static OSContext IdleContext;

OSThread* __OSCurrentThread     : (OS_BASE_CACHED | 0x00E4);
OSThreadQueue __OSActiveThreadQueue : (OS_BASE_CACHED | 0x00DC);

static OSSwitchThreadCallback SwitchThreadCallback = DefaultSwitchThreadCallback;

void UnsetRun(OSThread *) __attribute__((noinline));

#define EnqueueTail(queue, thread, link)                    \
do {                                                        \
    OSThread* __prev;                                       \
                                                            \
    __prev = (queue)->tail;                                 \
    if (__prev == NULL)                                     \
        (queue)->head = (thread);                           \
    else                                                    \
        __prev->link.next = (thread);                       \
    (thread)->link.prev = __prev;                           \
    (thread)->link.next = NULL;                             \
    (queue)->tail = (thread);                               \
} while (0)

#define EnqueuePrio(queue, thread, link)                    \
do {                                                        \
    OSThread* __prev;                                       \
    OSThread* __next;                                       \
                                                            \
    for (__next = (queue)->head;                            \
         __next && __next->priority <= thread->priority;    \
         __next = __next->link.next)                        \
            ;                                               \
    if (__next == NULL)                                     \
        EnqueueTail(queue, thread, link);                   \
    else {                                                  \
        (thread)->link.next = __next;                       \
        __prev = __next->link.prev;                         \
        __next->link.prev = (thread);                       \
        (thread)->link.prev = __prev;                       \
        if (__prev == NULL)                                 \
            (queue)->head = (thread);                       \
        else                                                \
            __prev->link.next = (thread);                   \
    }                                                       \
} while (0)

#define DequeueItem(queue, thread, link)                    \
do {                                                        \
    OSThread* __next;                                       \
    OSThread* __prev;                                       \
                                                            \
    __next = (thread)->link.next;                           \
    __prev = (thread)->link.prev;                           \
                                                            \
    if (__next == NULL)                                     \
        (queue)->tail = __prev;                             \
    else                                                    \
        __next->link.prev = __prev;                         \
                                                            \
    if (__prev == NULL)                                     \
        (queue)->head = __next;                             \
    else                                                    \
        __prev->link.next = __next;                         \
} while (0)

#define DequeueHead(queue, thread, link)                    \
do {                                                        \
    OSThread* __next;                                       \
                                                            \
    (thread) = (queue)->head;                               \
    __next = (thread)->link.next;                           \
    if (__next == NULL)                                     \
        (queue)->tail = NULL;                               \
    else                                                    \
        __next->link.prev = NULL;                           \
    (queue)->head = __next;                                 \
} while (0)

void DefaultSwitchThreadCallback(OSThread *, OSThread *) {
    return;
}

static inline void OSSetCurrentThread(OSThread *thread) {
    SwitchThreadCallback(__OSCurrentThread, thread);
    __OSCurrentThread = thread;
}

void OSInitThreadQueue(OSThreadQueue *queue) {
    queue->head = queue->tail = 0;
}

OSThread* OSGetCurrentThread(void) {
    return __OSCurrentThread;
}

static void __OSSwitchThread(OSThread *nextThread) {
    OSSetCurrentThread(nextThread);
    OSSetCurrentContext(&nextThread->context);
    OSLoadContext(&nextThread->context);
}

BOOL OSIsThreadSuspended(OSThread *thread) {
    return (0 < thread->suspend) ? TRUE : FALSE;
}

BOOL OSIsThreadTerminated(OSThread *thread) {
    return ((thread->state == 8) || (thread->state == 0)) ? TRUE : FALSE;
}

s32 OSDisableScheduler(void) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = Reschedule++;
    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSEnableScheduler(void) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = Reschedule--;
    OSRestoreInterrupts(enabled);
    return count;
}

static void SetRun(OSThread *thread) {
    thread->queue = &RunQueue[thread->priority];
    EnqueueTail(thread->queue, thread, link);
    RunQueueBits |= 1 << (31 - thread->priority);
    RunQueueHint = TRUE;
}

void UnsetRun(OSThread *thread) {
    OSThreadQueue* queue;
    queue = thread->queue;
    DequeueItem(queue, thread, link);

    if (queue->head == 0) {
        RunQueueBits &= ~(1 << (31 - thread->priority));
    }

    thread->queue = 0;
}

OSPriority __OSGetEffectivePriority(OSThread *thread) {
    OSPriority priority;
    OSMutex* mutex;
    OSThread* blocked;

    priority = thread->base;

    for (mutex = thread->queueMutex.head; mutex; mutex = mutex->link.next) {
        blocked = mutex->queue.head;

        if (blocked != 0 && blocked->priority < priority) {
            priority = blocked->priority;
        }
    }

    return priority;
}

OSThread* SetEffectivePriority(OSThread *thread, OSPriority priority) {
    switch (thread->state) {
        case 1:
            UnsetRun(thread);
            thread->priority = priority;
            SetRun(thread);
            break;
        case 4:
            DequeueItem(thread->queue, thread, link);
            thread->priority = priority;
            EnqueuePrio(thread->queue, thread, link);

            if (thread->mutex != 0) {
                return thread->mutex->thread;
            }
            break;
        case 2:
            RunQueueHint = TRUE;
            thread->priority = priority;
            break;
    }

    return NULL;
}

void __OSPromoteThread(OSThread *thread, OSPriority priority) {
    do {
        if ((thread->suspend < 0) || thread->priority <= priority) {
            break;
        }

        thread = SetEffectivePriority(thread, priority);
    } while (thread);
}

OSThread* SelectThread(BOOL yield) {
    OSContext* currentContext;
    OSThread* currentThread;
    OSThread* nextThread;
    OSPriority priority;
    OSThreadQueue* queue;

    if (Reschedule < 0) {
        return 0;
    }

    currentContext = OSGetCurrentContext();
    currentThread = OSGetCurrentThread();

    if (currentContext != &currentThread->context) {
        return 0;
    }

    if (currentThread != 0) {
        if (currentThread->state == 2) {
            if (!yield) {
                priority = __cntlzw(RunQueueBits);

                if (currentThread->priority <= priority) {
                    return 0;
                }
            }

            currentThread->state = 1;
            SetRun(currentThread);
        }

        if (!(currentThread->context.state & 2) && OSSaveContext(&currentThread->context)) {
            return 0;
        }
    }

    if (RunQueueBits == 0) {
        OSSetCurrentThread(0);
        OSSetCurrentContext(&IdleContext);

        do {
            OSEnableInterrupts();
            while (RunQueueBits == 0);
            OSDisableInterrupts();
        } while (RunQueueBits == 0);

        OSClearContext(&IdleContext);
    }

    RunQueueHint = FALSE;
    priority = __cntlzw(RunQueueBits);
    queue = &RunQueue[priority];
    DequeueHead(queue, nextThread, link);

    if (queue->head == 0) {
        RunQueueBits &= ~(1 << (31 - priority));
    }

    nextThread->queue = 0;
    nextThread->state = 2;
    __OSSwitchThread(nextThread);
    return nextThread;
}

void __OSReschedule(void) {
    if (RunQueueHint) {
        SelectThread(FALSE);
    }
}

void OSYieldThread(void) {
    BOOL enabled;
    enabled = OSDisableInterrupts();
    SelectThread(TRUE);
    OSRestoreInterrupts(enabled);
}

// OSCreateThread