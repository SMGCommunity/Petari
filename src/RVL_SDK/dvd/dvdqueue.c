#include <revolution/os.h>
#include <revolution/dvd.h>

typedef struct  {
    DVDCommandBlock* next;
    DVDCommandBlock* prev;
} Queue;

static Queue WaitingQueue[4];

void __DVDClearWaitingQueue(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        DVDCommandBlock* q = (DVDCommandBlock*)&(WaitingQueue[i]);
        q->next = q;
        q->prev = q;
    }
}

BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block) {
    BOOL enabled;
    DVDCommandBlock* q;

    enabled = OSDisableInterrupts();

    q = (DVDCommandBlock*)&(WaitingQueue[prio]);
    q->prev->next = block;
    block->prev = q->prev;
    block->next = q;
    q->prev = block;

    OSRestoreInterrupts(enabled);
    return TRUE;
}

static DVDCommandBlock* PopWaitingQueuePrio(s32 prio) {
    DVDCommandBlock* tmp, *q;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    q = (DVDCommandBlock*)&(WaitingQueue[prio]);

    tmp = q->next;
    q->next = tmp->next;
    tmp->next->prev = q;

    OSRestoreInterrupts(enabled);

    tmp->next = NULL;
    tmp->prev = NULL;
    return tmp;
}

DVDCommandBlock* __DVDPopWaitingQueue(void) {
    u32 i;
    BOOL enabled;
    DVDCommandBlock* q;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        q = (DVDCommandBlock*)&(WaitingQueue[i]);

        if (q->next != q) {
            OSRestoreInterrupts(enabled);
            return PopWaitingQueuePrio(i);
        }
    }

    OSRestoreInterrupts(enabled);
    return NULL;
}

BOOL __DVDCheckWaitingQueue(void) {
    u32 i;
    BOOL enabled;
    DVDCommandBlock* q;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        q = (DVDCommandBlock*)&(WaitingQueue[i]);

        if (q->next != q) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}

DVDCommandBlock* __DVDGetNextWaitingQueue(void) {
    u32 i;
    BOOL enabled;
    DVDCommandBlock* q, *tmp;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        q = (DVDCommandBlock*)&(WaitingQueue[i]);

        if (q->next != q) {
            tmp = q->next;
            OSRestoreInterrupts(enabled);
            return tmp;
        }
    }

    OSRestoreInterrupts(enabled);
    return NULL;
}

BOOL __DVDDequeueWaitingQueue(DVDCommandBlock* block) {
    BOOL enabled;
    DVDCommandBlock* prev, *next;

    enabled = OSDisableInterrupts();

    prev = block->prev;
    next = block->next;

    if (prev == NULL || next == NULL) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    prev->next = next;
    next->prev = prev;
    OSRestoreInterrupts(enabled);
    return TRUE;
}