#include "utils/common/CircleBuffer.h"
#include <revolution/db.h>
#include <revolution/exi.h>

#define GDEV_BUF_SIZE (0x500)

static CircleBuffer gRecvCB;

static u8 gRecvBuf[GDEV_BUF_SIZE];

static BOOL gIsInitialized;

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
    int p1;
    u32 retval;
    int p2;
    int poll;
    retval = 0;
    if (!gIsInitialized) {
        return -0x2711;
    }

    p1 = size;
    p2 = size;
    while ((u32)CBGetBytesAvailableForRead(&gRecvCB) < p2) {
        retval = 0;
        poll = DBQueryData();
        if (poll != 0) {
            retval = DBRead(buff, p2);
            if (retval == 0) {
                CircleBufferWriteBytes(&gRecvCB, buff, poll);
            }
        }
    }

    if (retval == 0) {
        CircleBufferReadBytes(&gRecvCB, data, p1);
    } else {
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
