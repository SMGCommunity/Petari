#ifndef MSGBUF_H
#define MSGBUF_H

#include "portable/mutex_TRK.h"
#include <size_t.h>

typedef int MessageBufferID;

typedef struct MessageBuffer {
    DSMutex fMutex;
    bool fInUse;
    unsigned int fLength;
    unsigned int fPosition;
    unsigned char fData[0x880];
} MessageBuffer;

int TRKReadBuffer(MessageBuffer *, void *, size_t);
void TRKReleaseBuffer(MessageBufferID);
int TRKGetFreeBuffer(MessageBufferID *, MessageBuffer **);
int TRKSetBufferPosition(MessageBuffer *, unsigned int);
MessageBuffer* TRKGetBuffer(MessageBufferID);
int TRKAppendBuffer_ui8(MessageBuffer *, const ui8 *, int);

#endif // MSGBUF_H