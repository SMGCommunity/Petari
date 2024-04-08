#include "portable/msgbuf.h"
#include "portable/mem_TRK.h"
#include "portable/usr_put.h"
#include "portable/nubinit.h"

typedef struct MessageBuffers {
    MessageBuffer fBuffers[3];
} MessageBuffers;

MessageBuffers gTRKMsgBufs;

int TRKReadBuffer(MessageBuffer *buffer, void *data, size_t length) {
    int err = 0;
    unsigned int remaining;

    if (length == 0) { 
        return err;
    }

    remaining = buffer->fLength - buffer->fPosition;

    if (length > remaining) {
        err = 770;
        length = remaining;
    }

    TRK_memcpy(data, buffer->fData + buffer->fPosition, length);
    buffer->fPosition += length;
    return err;
}

int TRKReadBuffer1_ui32(MessageBuffer* buffer, ui32 *data) {
    int err;

    ui8* bigEndianData;
    ui8* byteData;
    ui8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (ui8*)data;
    }
    else {
        bigEndianData = swapBuffer;
    }

    err = TRKReadBuffer(buffer, (void*)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && err == 0) {
        byteData = (ui8*)data;

        byteData[0] = bigEndianData[3];
        byteData[1] = bigEndianData[2];
        byteData[2] = bigEndianData[1];
        byteData[3] = bigEndianData[0];
    }

    return err;
}

int TRKReadBuffer_ui32(MessageBuffer *buffer, ui32* data, int count) {
    int err, i;

    for (i = 0, err = 0; err == 0 && i < count; i++) {
        err = TRKReadBuffer1_ui32(buffer, &(data[i]));
    }

    return err;
}

int TRKReadBuffer1_ui8(MessageBuffer *buffer, ui8 *data) {
    return TRKReadBuffer(buffer, (void*)data, 1);
}

int TRKReadBuffer_ui8(MessageBuffer *buffer, ui8* data, int count) {
    int err, i;

    for (i = 0, err = 0; err == 0 && i < count; i++) {
        err = TRKReadBuffer1_ui8(buffer, &(data[i]));
    }

    return err;
}

int TRKReadBuffer1_ui64(MessageBuffer *buffer, ui64* data) {
    int err;

    ui8* bigEndianData;
    ui8* byteData;
    ui8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (ui8*)data;
    }
    else {
        bigEndianData = swapBuffer;
    }

    err = TRKReadBuffer(buffer, (void*)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && err == 0) {
        byteData = (ui8*)data;

        byteData[0] = bigEndianData[7];
        byteData[1] = bigEndianData[6];
        byteData[2] = bigEndianData[5];
        byteData[3] = bigEndianData[4];
        byteData[4] = bigEndianData[3];
        byteData[5] = bigEndianData[2];
        byteData[6] = bigEndianData[1];
        byteData[7] = bigEndianData[0];
    }

    return err;
}

int TRKAppendBuffer(MessageBuffer *buffer, const void *data, size_t length) {
    int err = 0;
    unsigned int remaining;

    if (length == 0) {
        return err;
    }

    remaining = 0x880 - buffer->fPosition;

    if (remaining < length) {
        err = 769;
        length = remaining;
    }

    if (length == 1) {
        buffer->fData[buffer->fPosition] = *(unsigned char *)data;
    }
    else {
        TRK_memcpy(buffer->fData + buffer->fPosition, data, length);
    }

    buffer->fPosition += length;
    buffer->fLength = buffer->fPosition;
    return err;
}

int TRKAppendBuffer1_ui32(MessageBuffer *buffer, const ui32 data) {
    ui8* bigEndianData;
    ui8* byteData;
    ui8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (ui8*)&data;
    }
    else {
        byteData = (ui8*)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[3];
        bigEndianData[1] = byteData[2];
        bigEndianData[2] = byteData[1];
        bigEndianData[3] = byteData[0];
    }

    return TRKAppendBuffer(buffer, (const void*)bigEndianData, sizeof(data));
}

int TRKAppendBuffer_ui32(MessageBuffer *buffer, const ui32* data, int count) {
    int err, i;

    for (i = 0, err = 0; err == 0 && i < count; i++) {
        err = TRKAppendBuffer1_ui32(buffer, data[i]);
    }

    return err;
}

int TRKAppendBuffer1_ui8(MessageBuffer *buffer, const ui8 data) {
    if (buffer->fPosition >= 0x880) {
        return 769;
    }

    buffer->fData[buffer->fPosition++] = data;
    buffer->fLength++;
    return 0;
}

int TRKAppendBuffer_ui8(MessageBuffer *buffer, const ui8 *data, int count) {
    int err, i;

    for (i = 0, err = 0; err == 0 && i < count; i++) {
        err = TRKAppendBuffer1_ui8(buffer, data[i]);
    }

    return err;
}

int TRKAppendBuffer1_ui64(MessageBuffer *buffer, const ui64 data) {
    ui8* bigEndianData;
    ui8* byteData;
    ui8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (ui8*)&data;
    }
    else {
        byteData = (ui8*)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[7];
        bigEndianData[1] = byteData[6];
        bigEndianData[2] = byteData[5];
        bigEndianData[3] = byteData[4];
        bigEndianData[4] = byteData[3];
        bigEndianData[5] = byteData[2];
        bigEndianData[6] = byteData[1];
        bigEndianData[7] = byteData[0];
    }

    return TRKAppendBuffer(buffer, (const void*)bigEndianData, sizeof(data));
}

int TRKAppendBuffer_ui64(MessageBuffer *buffer, const ui64 *data, int count) {
    int err, i;

    for (i = 0, err = 0; err == 0 && i < count; i++) {
        err = TRKAppendBuffer1_ui64(buffer, data[i]);
    }

    return err;
}

int TRKSetBufferPosition(MessageBuffer *buffer, unsigned int position) {
    int err = 0;

    if (position > 0x880) {
        err = 769;
    }
    else {
        buffer->fPosition = position;

        if (position > buffer->fLength) {
            buffer->fLength = position;
        }
    }

    return err;
}

void TRKResetBuffer(MessageBuffer *buffer, int option) {
    buffer->fLength = 0;
    buffer->fPosition = 0;

    if (option == 0) {
        TRK_memset(buffer->fData, 0, 0x880);
    }
}

void TRKReleaseBuffer(MessageBufferID bufferID) {
    MessageBuffer* b;

    if (bufferID == -1) {
        return;
    }

    if (bufferID >= 0 && bufferID < 3) {
        b = &gTRKMsgBufs.fBuffers[bufferID];
        TRKAcquireMutex(&b->fMutex);
        b->fInUse = 0;
        TRKReleaseMutex(&b->fMutex);
    }
}

void TRKSetBufferUsed(MessageBuffer *buffer, s32 flag) {
    buffer->fInUse = flag;
}

MessageBuffer* TRKGetBuffer(MessageBufferID bufferID) {
    MessageBuffer* result = 0;

    if (bufferID >= 0 && bufferID < 3) {
        result = &gTRKMsgBufs.fBuffers[bufferID];
    }

    return result;
}

int TRKGetFreeBuffer(MessageBufferID *resultBufferID, MessageBuffer ** resultBuffer) {
    int result = 768;
    MessageBufferID i;
    MessageBuffer* b;
    *resultBuffer = 0;

    for (i = 0 ; i < 3; i++) {
        b = TRKGetBuffer(i);
        TRKAcquireMutex(&b->fMutex);

        if (b->fInUse == 0) {
            TRKResetBuffer(b, 1);
            TRKSetBufferUsed(b, 1);
            *resultBuffer = b;
            *resultBufferID = i;
            result = 0;
            i = 3;
        }

        TRKReleaseMutex(&b->fMutex);        
    }

    if (result == 0x300) {
        usr_puts_serial("ERROR : No buffer available\n");
    }

    return result;
}

int TRKInitializeMessageBuffers(void) {
    int i;

    for (i = 0; i < 3; i++) {
        TRKInitializeMutex(&gTRKMsgBufs.fBuffers[i].fMutex);
        TRKAcquireMutex(&gTRKMsgBufs.fBuffers[i].fMutex);
        TRKSetBufferUsed(&gTRKMsgBufs.fBuffers[i], 0);
        TRKReleaseMutex(&gTRKMsgBufs.fBuffers[i].fMutex);
    }

    return 0;
}