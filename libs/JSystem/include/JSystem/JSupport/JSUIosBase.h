#pragma once

#include <revolution.h>

enum JSUStreamSeekFrom {
    
};

class JSUIosBase {
public:
    JSUIosBase() {
        mState = 0;
    }

    enum EIoState {

    };

    virtual ~JSUIosBase() {

    }

    void setState(EIoState);

    u8 mState; // _4
};