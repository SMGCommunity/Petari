#include <revolution/os.h>

extern OSErrorHandler __OSErrorTable[];

static void DefaultSwitchThreadCallback(OSThread *, OSThread *);

#define OFFSET(n, a) (((u32) (n)) & ((a) - 1))
#define TRUNC(n, a)  (((u32) (n)) & ~((a) - 1))
#define ROUND(n, a)  (((u32) (n) + (a) - 1) & ~((a) - 1))

static volatile u32 RunQueueBits;
static OSThreadQueue RunQueue[32];
static volatile BOOL RunQueueHint;

static volatile s32 Reschedule;
static OSThread IdleThread;
static OSThread DefaultThread;
static OSContext IdleContext;

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

#define IsSuspended(suspendCount)   (0 < (suspendCount))

static inline void OSInitMutexQueue(OSMutexQueue* queue) {
    queue->head = queue->tail = NULL;
}

void DefaultSwitchThreadCallback(OSThread *, OSThread *) {
    return;
}

static inline void OSSetCurrentThread(OSThread* thread) {
    SwitchThreadCallback(__OSCurrentThread, thread);
    __OSCurrentThread = thread;
}

extern u8 _stack_addr[];
extern u8 _stack_end[];

void __OSThreadInit(void) {
    OSThread* thread = &DefaultThread;
    int prio;

    thread->state = 2;
    thread->attr = 1;
    thread->priority = thread->base = 16;
    thread->suspend = 0;
    thread->value = (void*)-1;
    thread->mutex = NULL;
    OSInitThreadQueue(&thread->queueJoin);
    OSInitMutexQueue(&thread->queueMutex);
    __OSFPUContext = &thread->context;
    OSClearContext(&thread->context);
    OSSetCurrentContext(&thread->context);

    thread->stackBase = (void*)_stack_addr;
    thread->stackEnd = (void*)_stack_end;
    *(thread->stackEnd) = 0xDEADBABE;

    OSSetCurrentThread(thread);
    OSClearStack(0);

    RunQueueBits = 0;
    RunQueueHint = FALSE;

    for (prio = 0; prio <= 31; prio++) {
        OSInitThreadQueue(&RunQueue[prio]);
    }

    OSInitThreadQueue(&__OSActiveThreadQueue);
    EnqueueTail(&__OSActiveThreadQueue, thread, linkActive);
    OSClearContext(&IdleContext);
    Reschedule = 0;
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

static BOOL __OSIsThreadActive(OSThread* thread) {
    OSThread* active;

    if (thread->state == 0) {
        return FALSE;
    }

    for (active = __OSActiveThreadQueue.head; active; active = active->linkActive.next) {
        if (thread == active) {
            return TRUE;
        }
    }

    return FALSE;
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

static void UpdatePriority(OSThread* thread) {
    OSPriority priority;

    do {
        if (IsSuspended(thread->suspend)) {
            break;
        }
        priority = __OSGetEffectivePriority(thread);

        if (thread->priority == priority) {
            break;
        }
        thread = SetEffectivePriority(thread, priority);
    } while (thread);
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

BOOL OSCreateThread(OSThread* thread, void* (*func)(void *), void* param, void* stack, u32 stackSize, OSPriority priority, u16 attr) {
    BOOL enabled;
    u32 sp;
    int i;

    if (priority < 0 || 31 < priority) {
        return FALSE;
    }

    thread->state = 1;
    thread->attr = (u16)(attr & 1);
    thread->priority = thread->base = priority;
    thread->suspend = 1;
    thread->value = (void*)-1;
    thread->mutex = NULL;
    OSInitThreadQueue(&thread->queueJoin);
    OSInitMutexQueue(&thread->queueMutex);

    sp = (u32)stack;
    sp = TRUNC(sp, 8);
    sp -= 8;

    ((u32*)sp)[0] = 0;
    ((u32*)sp)[1] = 0;

    OSInitContext(&thread->context, (u32)func, sp);
    thread->context.lr = (u32)OSExitThread;
    thread->context.gpr[3] = (u32)param;
    thread->stackBase = stack;
    thread->stackEnd = (u32*)((u32)stack - stackSize);
    *(thread->stackEnd) = 0xDEADBABE;
    thread->error = 0;
    for (i = 0; i < 2; ++i) {
        thread->specific[i] = 0;
    }

    enabled = OSDisableInterrupts();
    if (__OSErrorTable[16] != NULL) {
        thread->context.srr1 |= 0x900;
        thread->context.state |= 1;
        thread->context.fpscr = (__OSFpscrEnableBits & 248) | 4;

        for (i = 0; i < 32; ++i) {
            *(u64*) &thread->context.fpr[i] = (u64)0xffffffffffffffffLL;
            *(u64*) &thread->context.psf[i] = (u64) 0xffffffffffffffffLL;
        }
    }

    EnqueueTail(&__OSActiveThreadQueue, thread, linkActive);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

void OSExitThread(void* val) {
    BOOL enabled;
    OSThread* currentThread;

    enabled = OSDisableInterrupts();
    currentThread = OSGetCurrentThread();
    OSClearContext(&currentThread->context);

    if (currentThread->attr & 1) {
        DequeueItem(&__OSActiveThreadQueue, currentThread, linkActive);
        currentThread->state = 0;
    }
    else {
        currentThread->state = 8;
        currentThread->value = val;
    }

    __OSUnlockAllMutex(currentThread);
    OSWakeupThread(&currentThread->queueJoin);
    RunQueueHint = TRUE;
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

void OSCancelThread(OSThread* thread) {
    BOOL enabled = OSDisableInterrupts();

    switch (thread->state) {
      case OS_THREAD_STATE_READY:
        if (!IsSuspended(thread->suspend)) {
            UnsetRun(thread);
        }
        break;

      case OS_THREAD_STATE_RUNNING:
        RunQueueHint = TRUE;
        break;

      case OS_THREAD_STATE_WAITING:
        DequeueItem(thread->queue, thread, link);
        thread->queue = NULL;
        if (!IsSuspended(thread->suspend) && thread->mutex) {
            ASSERT(thread->mutex->thread);
            UpdatePriority(thread->mutex->thread);
        }
        break;

      default:
        OSRestoreInterrupts(enabled);
        return;
    }

    OSClearContext(&thread->context);
    if (thread->attr & 1) {
        DequeueItem(&__OSActiveThreadQueue, thread, linkActive);
        thread->state = 0;
    } 
    else {
        thread->state = 8;
    }

    __OSUnlockAllMutex(thread);
    OSWakeupThread(&thread->queueJoin);
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

BOOL OSJoinThread(OSThread* thread, void** val) {
    BOOL enabled = OSDisableInterrupts();

    if (!(thread->attr & 1) && thread->state != OS_THREAD_STATE_MORIBUND && thread->queueJoin.head == NULL) {
        OSSleepThread(&thread->queueJoin);
        if (!__OSIsThreadActive(thread)) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }
    }

    if (((volatile OSThread*) thread)->state == OS_THREAD_STATE_MORIBUND) {
        if (val) {
            *val = thread->value;
        }

        DequeueItem(&__OSActiveThreadQueue, thread, linkActive);
        thread->state = 0;
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}

void OSDetachThread(OSThread* thread)
{
    BOOL enabled = OSDisableInterrupts();

    thread->attr |= 1;
    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        DequeueItem(&__OSActiveThreadQueue, thread, linkActive);
        thread->state = 0;
    }

    OSWakeupThread(&thread->queueJoin);
    OSRestoreInterrupts(enabled);
}

s32 OSResumeThread(OSThread* thread) {
    BOOL enabled;
    s32  suspendCount;

    enabled = OSDisableInterrupts();

    suspendCount = thread->suspend--;

    if (thread->suspend < 0) {
        thread->suspend = 0;
    }
    else if (thread->suspend == 0) {
        switch (thread->state) {
          case OS_THREAD_STATE_READY:
            thread->priority = __OSGetEffectivePriority(thread);
            SetRun(thread);
            break;

          case OS_THREAD_STATE_WAITING:
            ASSERT(thread->queue);
            DequeueItem(thread->queue, thread, link);
            thread->priority = __OSGetEffectivePriority(thread);
            EnqueuePrio(thread->queue, thread, link);
            if (thread->mutex) {
                UpdatePriority(thread->mutex->thread);
            }
            break;
        }

        __OSReschedule();
    }

    OSRestoreInterrupts(enabled);
    return suspendCount;
}

s32 OSSuspendThread(OSThread* thread)
{
    BOOL enabled;
    s32  suspendCount;

    enabled = OSDisableInterrupts();
    suspendCount = thread->suspend++;

    if (suspendCount == 0) {
        switch (thread->state) {
          case OS_THREAD_STATE_RUNNING:
            RunQueueHint = TRUE;
            thread->state = OS_THREAD_STATE_READY;
            break;

          case OS_THREAD_STATE_READY:
            UnsetRun(thread);
            break;

          case OS_THREAD_STATE_WAITING:
            DequeueItem(thread->queue, thread, link);
            thread->priority = 32;
            EnqueueTail(thread->queue, thread, link);

            if (thread->mutex) {
                ASSERT(thread->mutex->thread);
                UpdatePriority(thread->mutex->thread);
            }
            break;
        }

        __OSReschedule();
    }

    OSRestoreInterrupts(enabled);
    return suspendCount;
}

void OSSleepThread(OSThreadQueue* queue) {
    BOOL enabled;
    OSThread* currentThread;

    enabled = OSDisableInterrupts();
    currentThread = OSGetCurrentThread();

    currentThread->state = 4;
    currentThread->queue = queue;
    EnqueuePrio(queue, currentThread, link);
    RunQueueHint = TRUE;
    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

void OSWakeupThread(OSThreadQueue* queue) {
    BOOL enabled;
    OSThread* thread;

    enabled = OSDisableInterrupts();
    while (queue->head) {
        DequeueHead(queue, thread, link);
        thread->state = 1;

        if (!IsSuspended(thread->suspend)) {
            SetRun(thread);
        }
    }

    __OSReschedule();
    OSRestoreInterrupts(enabled);
}

static void OSClearStack(u8 val) {
    u32 sp;
    u32* p;
    u32 pattern;

    pattern = ((u32)val << 24) | ((u32)val << 16) | ((u32)val << 8) | (u32)val;
    sp = OSGetStackPointer();

    for (p = __OSCurrentThread->stackEnd + 1; p < (u32*)sp; ++p) {
        *p = pattern;
    }
}

BOOL OSSetThreadPriority(OSThread* thread, OSPriority priority) {
    BOOL enabled;

    if (priority < 0 || priority > 31) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();

    if (thread->base != priority) {
        thread->base = priority;
        UpdatePriority(thread);
        __OSReschedule();
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

OSPriority OSGetThreadPriority(OSThread* thread) {
    return thread->base;
}

static void SleepAlarmHandler(OSAlarm *alarm, OSContext *context) {
    OSResumeThread((OSThread*)OSGetAlarmUserData(alarm));
}

void OSSleepTicks(OSTime tick) {
    BOOL enabled;
    OSThread* current;
    OSAlarm sleepAlarm;

    enabled = OSDisableInterrupts();
    current = OSGetCurrentThread();

    if (current == NULL) {
        OSRestoreInterrupts(enabled);
        return;
    }

    OSCreateAlarm(&sleepAlarm);
    OSSetAlarmTag(&sleepAlarm, (u32)current);
    OSSetAlarmUserData(&sleepAlarm, (void*)current);
    OSSetAlarm(&sleepAlarm, tick, SleepAlarmHandler);
    OSSuspendThread(current);
    OSCancelAlarm(&sleepAlarm);
    OSRestoreInterrupts(enabled);
}
