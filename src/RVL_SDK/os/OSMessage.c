#include "revolution/os.h"

void OSInitMessageQueue(OSMessageQueue *mq, OSMessage* msgArray, s32 msgCount) {
    OSInitThreadQueue(&mq->queueSend);
    OSInitThreadQueue(&mq->queueReceive);
    mq->msgArray = msgArray;
    mq->msgCount = msgCount;
    mq->firstIndex = 0;
    mq->usedCount = 0;
}

BOOL OSSendMessage(OSMessageQueue* mq, OSMessage msg, s32 flags) {
    BOOL enabled;
    s32 lastIndex;

    enabled = OSDisableInterrupts();

    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }
        else {
            OSSleepThread(&mq->queueSend);
        }
    }

    lastIndex = (mq->firstIndex + mq->usedCount) % mq->msgCount;
    mq->msgArray[lastIndex] = msg;
    mq->usedCount++;
    OSWakeupThread(&mq->queueReceive);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL OSReceiveMessage(OSMessageQueue* mq, OSMessage* msg, s32 flags) {
    BOOL enabled = OSDisableInterrupts();

    while (mq->usedCount == 0) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }
        else {
            OSSleepThread(&mq->queueReceive);
        }
    }

    if (msg != NULL) {
        *msg = mq->msgArray[mq->firstIndex];
    }

    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;

    OSWakeupThread(&mq->queueSend);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL OSJamMessage(OSMessageQueue* mq, OSMessage msg, s32 flags) {
    BOOL enabled = OSDisableInterrupts();

    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            OSRestoreInterrupts(enabled);
            return FALSE;
        }
        else {
            OSSleepThread(&mq->queueSend);
        }
    }

    mq->firstIndex = (mq->firstIndex + mq->msgCount - 1) % mq->msgCount;
    mq->msgArray[mq->firstIndex] = msg;
    mq->usedCount++;

    OSWakeupThread(&mq->queueReceive);
    OSRestoreInterrupts(enabled);
    return TRUE;
}