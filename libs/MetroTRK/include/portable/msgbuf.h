#ifndef MSGBUF_H
#define MSGBUF_H

#include "portable/mutex_TRK.h"
#include "portable/dserror.h"
#include "TRK_Types.h"
#include <size_t.h>

typedef int MessageBufferID;

typedef struct MessageBuffer {
    DSMutex fMutex;
    int fInUse;
    unsigned int fLength;
    unsigned int fPosition;
    unsigned char fData[0x880];
} MessageBuffer;

DSError TRKReadBuffer(MessageBuffer *, void *, size_t);
void TRKReleaseBuffer(MessageBufferID);
DSError TRKGetFreeBuffer(MessageBufferID *, MessageBuffer **);
int TRKSetBufferPosition(MessageBuffer *, unsigned int);
MessageBuffer* TRKGetBuffer(MessageBufferID);
int TRKAppendBuffer_ui8(MessageBuffer *, const ui8 *, int);

DSError TRKAppendBuffer_ui32(MessageBuffer *buffer, const ui32* data, int count);
DSError TRKReadBuffer_ui32(MessageBuffer *buffer, ui32* data, int count);

#endif // MSGBUF_H