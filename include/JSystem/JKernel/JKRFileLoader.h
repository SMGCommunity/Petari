#pragma once

#include "JSystem/JKernel/JKRDisposer.h"

class JKRFileLoader : public JKRDisposer {
public:
    JKRFileLoader();

    virtual ~JKRFileLoader();
    virtual void unmount();

    JSULink<JKRFileLoader> _18;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
};