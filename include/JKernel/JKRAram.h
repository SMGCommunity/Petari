#pragma once

#include "JKernel/JKRThread.h"

class JKRAram : public JKRThread
{
public:
    JKRAram(u32, u32, s32);

    static JKRAram* create(u32, u32, s32, s32, s32);

    static JKRAram* sAramObject;
};