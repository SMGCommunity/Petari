#include "utils/common/CircleBuffer.h"
#include <revolution/db.h>
#include <revolution/exi.h>

#define GDEV_BUF_SIZE (0x500)

static CircleBuffer gRecvCB;

static u8 gRecvBuf[GDEV_BUF_SIZE];

static BOOL gIsInitialized;

void OutputData(void* data, int length) {
    // u8 byte;
    int i;
    u8* datapointer = data;

    for (i = 0; i < length; i++) {
        if (i % 16 == 15) {
        }
    }
}

int gdev_cc_initialize(void* inputPendingPtrRef, EXICallback monitorCallback) {
    DBInitComm(inputPendingPtrRef, (int*)monitorCallback);
    CircleBufferInitialize(&gRecvCB, gRecvBuf, GDEV_BUF_SIZE);
    return 0;
}

int gdev_cc_shutdown() {
    return 0;
}

int gdev_cc_open() {
    if (gIsInitialized != 0) {
        return -10005;
    }

    gIsInitialized = TRUE;
    return 0;
}

int gdev_cc_close() {
    return 0;
}

int gdev_cc_read(u8* data, int size) {
    u8 buff[GDEV_BUF_SIZE];
    u32 retval = 0;
    s32 s;
    u32 poll;

    if (!gIsInitialized) {
        return -0x2711;
    }

    s = size;

    while (CBGetBytesAvailableForRead(&gRecvCB) < s) {
        retval = 0;
        poll = DBQueryData();
        if (poll != 0) {
            retval = DBRead(buff, s);
            if (retval == 0) {
                CircleBufferWriteBytes(&gRecvCB, buff, poll);
            }
        }
    }

    if (retval == 0) {
        CircleBufferReadBytes(&gRecvCB, data, s);
    }

    return retval;
}

int gdev_cc_write(const u8* bytes, int length) {
    int exi2Len;
    int n_copy;
    u32 hexCopy;

    hexCopy = (u32)bytes;
    n_copy = length;

    if (gIsInitialized == FALSE) {
        return -0x2711;
    }

    while (n_copy > 0) {
        exi2Len = DBWrite((const void*)hexCopy, n_copy);
        if (exi2Len == 0) {
            break;
        }
        hexCopy += exi2Len;
        n_copy -= exi2Len;
    }

    return 0;
}

int gdev_cc_pre_continue() {
    DBClose();
    return 0;
}

int gdev_cc_post_stop() {
    DBOpen();
    return 0;
}

int gdev_cc_peek() {
    int poll;
    u8 buff[GDEV_BUF_SIZE];

    poll = DBQueryData();
    if (poll <= 0) {
        return 0;
    }

    if ((int)DBRead(buff, poll) == 0) {
        CircleBufferWriteBytes(&gRecvCB, buff, poll);
    } else {
        return -0x2719;
    }

    return poll;
}

int gdev_cc_initinterrupts() {
    DBInitInterrupts();
    return 0;
}
