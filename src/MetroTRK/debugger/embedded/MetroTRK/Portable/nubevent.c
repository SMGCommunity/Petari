#include "portable/nubevent.h"
#include "portable/mutex_TRK.h"
#include "portable/mem_TRK.h"

typedef struct EventQueue {
    DSMutex fMutex;
    int fCount;
    int fFirst;
    NubEvent fEventList[2];
    NubEventID fEventID;
} EventQueue;

EventQueue gTRKEventQueue;

void TRKDestructEvent(NubEvent* oldEvent) {
    TRKReleaseBuffer(oldEvent->fMessageBufferID);
}

void TRKConstructEvent(NubEvent *newEvent, NubEventType type) {
    newEvent->fType = type;
    newEvent->fID = 0;
    newEvent->fMessageBufferID = -1;
}

void TRKCopyEvent(NubEvent *dstEvent, const NubEvent *srcEvent) {
    TRK_memcpy(dstEvent, srcEvent, sizeof(*dstEvent));
}

int TRKPostEvent(const NubEvent *copiedEvent) {
    int result = 0;
    int nextEntry;

    TRKAcquireMutex(&gTRKEventQueue.fMutex);

    if (gTRKEventQueue.fCount == 2) {
        result = 0x100;
    }
    else {
        nextEntry = (gTRKEventQueue.fFirst + gTRKEventQueue.fCount) % 2;
        TRKCopyEvent(&gTRKEventQueue.fEventList[nextEntry], copiedEvent);

        gTRKEventQueue.fEventList[nextEntry].fID = gTRKEventQueue.fEventID;
        gTRKEventQueue.fEventID++;

        if (gTRKEventQueue.fEventID < 0x100) {
            gTRKEventQueue.fEventID = 0x100;
        }

        gTRKEventQueue.fCount++;
    }

    TRKReleaseMutex(&gTRKEventQueue.fMutex);
    return result;
}

s32 TRKGetNextEvent(NubEvent* resultEvent) {
    s32 result = 0;

    TRKAcquireMutex(&gTRKEventQueue.fMutex);

    if (gTRKEventQueue.fCount > 0) {
        TRKCopyEvent(resultEvent, &gTRKEventQueue.fEventList[gTRKEventQueue.fFirst]);
        gTRKEventQueue.fCount--;
        gTRKEventQueue.fFirst++;

        if (gTRKEventQueue.fFirst == 2) {
            gTRKEventQueue.fFirst = 0;
        }

        result = 1;
    }

    TRKReleaseMutex(&gTRKEventQueue.fMutex);
    return result;
}

int TRKInitializeEventQueue(void) {
    TRKInitializeMutex(&gTRKEventQueue.fMutex);
    TRKAcquireMutex(&gTRKEventQueue.fMutex);
    gTRKEventQueue.fCount = 0;
    gTRKEventQueue.fFirst = 0;
    gTRKEventQueue.fEventID = 0x100;
    TRKReleaseMutex(&gTRKEventQueue.fMutex);
    return 0;
}