#ifndef OSMESSAGE_H
#define OSMESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/os/OSThread.h>

typedef struct OSMessageQueue OSMessageQueue;
typedef void* OSMessage;

struct OSMessageQueue {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    OSMessage* msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
};

#define OS_MESSAGE_NOBLOCK  0
#define OS_MESSAGE_BLOCK    1

void OSInitMessageQueue(OSMessageQueue *, OSMessage *, s32);
BOOL OSSendMessage(OSMessageQueue *, OSMessage, s32);
BOOL OSJamMessage(OSMessageQueue *, OSMessage, s32);
BOOL OSReceiveMessage(OSMessageQueue *, OSMessage *, s32);

#ifdef __cplusplus
}
#endif

#endif // OSMESSAGE_H
