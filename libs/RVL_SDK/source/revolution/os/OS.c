#include <revolution/os.h>

static __OSExceptionHandler* OSExceptionTable;

__OSExceptionHandler __OSSetExceptionHandler(__OSException ex, __OSExceptionHandler handler) {
    __OSExceptionHandler cur;
    cur = OSExceptionTable[ex];
    OSExceptionTable[ex] = handler;
    return cur;
}

__OSExceptionHandler __OSGetExceptionHandler(__OSException ex) {
    return OSExceptionTable[ex];
}