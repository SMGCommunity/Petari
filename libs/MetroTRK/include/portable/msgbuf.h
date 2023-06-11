#ifndef MSGBUF_H
#define MSGBUF_H

#include "portable/mutex_TRK.h"

typedef int MessageBufferID;

typedef struct MessageBuffer {
    DSMutex fMutex;
    bool fInUse;
    unsigned int fLength;
    unsigned int fPosition;
    unsigned char fData[0x800];
} MessageBuffer;

void TRKReleaseBuffer(MessageBufferID);

#endif // MSGBUF_H