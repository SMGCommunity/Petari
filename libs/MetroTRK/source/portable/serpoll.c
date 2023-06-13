#include "portable/msgbuf.h"
#include "portable/msghndlr.h"
#include "portable/nubevent.h"
#include "portable/serframe.h"
#include "export/UART.h"

typedef enum ReceiverState {
	kWaitFlag,
	kFoundFlag,
	kInFrame,
	kFrameOverflow
} ReceiverState;

typedef struct FramingState {
    MessageBufferID fBufferID;
    MessageBuffer* fBuffer;
    ReceiverState fReceiveState;
    bool fEscape;
    FCSType fFCS;
} FramingState;

static FramingState gTRKFramingState;

int TRKInitializeSerialHandler(void) {
    gTRKFramingState.fBufferID = -1;
    gTRKFramingState.fReceiveState = 0;
    gTRKFramingState.fEscape = 0;
    return 0;
}

int TRKTerminateSerialHandler(void) {
    return 0;
}

void TRKProcessInput(MessageBufferID bufferID) {
    NubEvent event;
    TRKConstructEvent(&event, kRequestEvent);
    event.fMessageBufferID = bufferID;
    gTRKFramingState.fBufferID = -1;
    TRKPostEvent(&event);
}

void TRKGetInput(void) {
    MessageBuffer* msgBuffer;
    MessageBufferID bufferID;
    NubEvent event;

    bufferID = TRKTestForPacket();

    if (bufferID != -1) {
        msgBuffer = TRKGetBuffer(bufferID);
        TRKConstructEvent(&event, 2);
        event.fMessageBufferID = bufferID;
        gTRKFramingState.fBufferID = -1;
        TRKPostEvent(&event);
    }
}

MessageBufferID TRKTestForPacket(void) {
    int dbgErr, err;
    MessageBufferID id;
    MessageBuffer* b;
    int bytes, i;

    bytes = TRKPollUART();

    if (bytes > 0) {
        dbgErr = TRKGetFreeBuffer(&id, &b);
        TRKSetBufferPosition(b, 0);

        err = TRKReadUARTN(b->fData, 0x40);

        if (err == 0) {
            TRKAppendBuffer_ui8(b, b->fData, 0x40);
        }
    }
}