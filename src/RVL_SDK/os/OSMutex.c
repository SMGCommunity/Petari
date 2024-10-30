#include <revolution/os.h>

#define EnqueueTail(queue, mutex, link)         \
do {                                            \
    OSMutex* __prev;                            \
                                                \
    __prev = (queue)->tail;                     \
    if (__prev == NULL)                         \
        (queue)->head = (mutex);                \
    else                                        \
        __prev->link.next = (mutex);            \
    (mutex)->link.prev = __prev;                \
    (mutex)->link.next = NULL;                  \
    (queue)->tail = (mutex);                    \
} while (0)

#define DequeueItem(queue, mutex, link)         \
do {                                            \
    OSMutex* __next;                            \
    OSMutex* __prev;                            \
                                                \
    __next = (mutex)->link.next;                \
    __prev = (mutex)->link.prev;                \
                                                \
    if (__next == NULL)                         \
        (queue)->tail = __prev;                 \
    else                                        \
        __next->link.prev = __prev;             \
                                                \
    if (__prev == NULL)                         \
        (queue)->head = __next;                 \
    else                                        \
        __prev->link.next = __next;             \
} while (0)

#define DequeueHead(queue, mutex, link)         \
do {                                            \
    OSMutex* __next;                            \
                                                \
    (mutex) = (queue)->head;                    \
    __next = (mutex)->link.next;                \
    if (__next == NULL)                         \
        (queue)->tail = NULL;                   \
    else                                        \
        __next->link.prev = NULL;               \
    (queue)->head = __next;                     \
} while (0)

void OSInitMutex(OSMutex* mutex) {
    OSInitThreadQueue(&mutex->queue);
    mutex->thread = NULL;
    mutex->count = 0;
}

void OSLockMutex(OSMutex* mutex) {
    BOOL enabled;
    OSThread* currentThread, *ownerThread;

    enabled = OSDisableInterrupts();
    currentThread = OSGetCurrentThread();

    for (;;) {
        ownerThread = ((volatile OSMutex*)mutex)->thread;

        if (ownerThread == NULL) {
            mutex->thread = currentThread;
            mutex->count++;
            EnqueueTail(&currentThread->queueMutex, mutex, link);
            break;
        }
        else if (ownerThread == currentThread) {
            mutex->count++;
            break;
        }
        else {
            currentThread->mutex = mutex;
            __OSPromoteThread(mutex->thread, currentThread->priority);
            OSSleepThread(&mutex->queue);
            currentThread->mutex = NULL;
        }
    }

    OSRestoreInterrupts(enabled);
}

void OSUnlockMutex(OSMutex* mutex) {
    BOOL enabled;
    OSThread* currentThread;

    enabled = OSDisableInterrupts();
    currentThread = OSGetCurrentThread();

    if (mutex->thread == currentThread && --mutex->count == 0) {
        DequeueItem(&currentThread->queueMutex, mutex, link);
        mutex->thread = NULL;
        if (currentThread->priority < currentThread->base) {
            currentThread->priority = __OSGetEffectivePriority(currentThread);
        }

        OSWakeupThread(&mutex->queue);
    }

    OSRestoreInterrupts(enabled);
}

void __OSUnlockAllMutex(OSThread* thread) {
    OSMutex* mutex;
    while (thread->queueMutex.head) {
        DequeueHead(&thread->queueMutex, mutex, link);
        ASSERT(mutex->thread == thread);
        mutex->count = 0;
        mutex->thread = NULL;
        OSWakeupThread(&mutex->queue);
    }
}

BOOL OSTryLockMutex(OSMutex* mutex) {
    BOOL enabled, locked;
    OSThread* currentThread;

    enabled = OSDisableInterrupts();
    currentThread = OSGetCurrentThread();

    if (mutex->thread == NULL) {
        mutex->thread = currentThread;
        mutex->count++;
        EnqueueTail(&currentThread->queueMutex, mutex, link);
        locked = TRUE;
    }
    else if (mutex->thread == currentThread) {
        mutex->count++;
        locked = TRUE;
    }
    else {
        locked = FALSE;
    }

    OSRestoreInterrupts(enabled);
    return locked;
}