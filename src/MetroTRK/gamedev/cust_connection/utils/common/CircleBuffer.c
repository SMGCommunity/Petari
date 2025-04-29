#include "TRK_Types.h"
#include "size_t.h"

typedef s32 CBError;

CBError CircleBufferReadBytes(u32 *buffer, u8 *data, size_t length) {
    size_t size;

    if (length > buffer[4]) {
        return -1;
    }

    MWEnterCriticalSection(&buffer[6]);
    size = buffer[3] - (buffer[0] - buffer[2]);

    if (length < size) {
        memcpy(data, (void *)buffer[0], length);
        buffer[0] += length;
    }
    else {
        memcpy(data, (void *)buffer[0], size);
        memcpy(&data[size], (void *)buffer[2], (length - size));
        buffer[0] = ((buffer[2] + length) - size);
    }

    if (buffer[3] == (buffer[0] - buffer[2])) {
        buffer[0] = buffer[2];
    }

    buffer[5] += length;
    buffer[4] -= length;
    MWExitCriticalSection(&buffer[6]);

    return 0;
}

CBError CircleBufferWriteBytes(u32 *buffer, u8 *data, size_t length) {
    size_t size;

    if (length > buffer[5]) {
        return -1;
    }

    MWEnterCriticalSection(&buffer[6]);
    size = buffer[3] - (buffer[1] - buffer[2]);

    if (size >= length) {
        memcpy((void *)buffer[1], data, length);
        buffer[1] += length;
    }
    else {
        memcpy((void *)buffer[1], data, size);
        memcpy((void *)buffer[2], &data[size], (length - size));
        buffer[1] = ((buffer[2] + length) - size);
    }

    if (buffer[3] == (buffer[1] - buffer[2])) {
        buffer[1] = buffer[2];
    }

    buffer[5] -= length;
    buffer[4] += length;
    MWExitCriticalSection(&buffer[6]);

    return 0;
}

void CircleBufferInitialize(u32 *dest, u32 src, u32 buffersize) {
    dest[2] = src;
    dest[3] = buffersize;
    dest[0] = dest[2];
    dest[1] = dest[2];
    dest[4] = 0;
    dest[5] = dest[3];

    MWInitializeCriticalSection(&dest[6]);
}

u32 CBGetBytesAvailableForRead(u32 *buffer) {
    return buffer[4];
}