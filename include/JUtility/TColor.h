#pragma once

#include <revolution.h>

namespace JUtility
{
    class TColor
    {
    public:
        TColor(u8, u8, u8, u8);
        void operator=(const TColor &);

        u8 red; // _0
        u8 green; // _1
        u8 blue; // _2
        u8 alpha; // _3
    };
};