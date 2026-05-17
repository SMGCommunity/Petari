#pragma once

#include <revolution.h>

enum JSUStreamSeekFrom { SEEK_FROM_START, SEEK_FROM_POSITION, SEEK_FROM_END };

class JSUIosBase {
public:
    enum EIoState { IO_OK = 0b00000000, IO_ERROR = 0b00000001, IO_MEMORY_ERROR = 0b00000010 };

    JSUIosBase() {
        mState = IO_OK;
    }

    virtual ~JSUIosBase() {
    }

    bool isGood() const {
        return mState == IO_OK;
    }

    void setState(EIoState state) {
        mState |= state;
    }

    void clearState(EIoState state) {
        mState &= state;
    }

    u8 getState(EIoState state) {
        return mState & state;
    }

    bool mState;  // 0x4
};
