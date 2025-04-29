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
    int fEscape;
    FCSType fFCS;
} FramingState;

static void* gTRKInputPendingPtr;
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
    MessageBufferID bufferID;
    MessageBufferID replyID;
    MessageBuffer *msgbuf;
    int data[16];

    if (TRKPollUART() <= kNoError) {
        return -1;
    }

    replyID = TRKGetFreeBuffer(&bufferID, &msgbuf);
    TRKSetBufferPosition(msgbuf, 0);

    if (TRKReadUARTN(data, 0x40) == 0) {
        TRKAppendBuffer_ui8(msgbuf, (const u8*)data, 0x40);
        replyID = bufferID;

        if ((data[0] - 0x40 ) > 0) {
            char buffer[0x880];
            if (TRKReadUARTN(buffer, (u32)(data[0] - 0x40)) == 0) {
                TRKAppendBuffer_ui8(msgbuf, (const u8*)buffer, data[0]);
            }
            else {
                TRKReleaseBuffer(replyID);
                replyID = -1;
            }
        }
    }
    else {
        TRKReleaseBuffer(replyID);
        replyID = -1;
    }

    return replyID;
}
