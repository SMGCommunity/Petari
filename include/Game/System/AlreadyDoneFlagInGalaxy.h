#pragma once

#include "types.h"

class AlreadyDoneInfo
{
public:
    AlreadyDoneInfo();

    void clear();
    void init(const char *, s32, s32);
    bool isEqual(const AlreadyDoneInfo &) const;
    void set(bool);

    s16 _0;
    u16 _2;
    u16 _4;
};