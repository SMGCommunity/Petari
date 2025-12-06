#pragma once

#include <revolution.h>

enum JSUStreamSeekFrom { SEEK_FROM_START, SEEK_FROM_POSITION, SEEK_FROM_END };

class JSUIosBase {
public:
    JSUIosBase() { mState = 0; }

    enum EIoState { IO_OK, IO_ERROR };

    virtual ~JSUIosBase() {}

    void setState(EIoState);

    void clearState(EIoState state) { mState &= ~state; }

    s8 mState;  // 0x4
};