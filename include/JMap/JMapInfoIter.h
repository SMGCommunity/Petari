#pragma once

#include "types.h"

class JMapInfoIter
{
public:
    bool isValid() const;

    template<typename T>
    bool getValue(const char *, T) const;

    u32 _0;
    s32 _4;
};