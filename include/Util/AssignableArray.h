#pragma once

#include <revolution.h>

namespace MR
{
    template<typename T>
    class AssignableArray
    {
    public:
        ~AssignableArray();

        T* mContent; // _0
        s32 mCount; // _4
    };
};