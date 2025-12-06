#pragma once

#include <revolution.h>

class JASOscillator {
public:
    struct Point {
        const Point& operator=(const Point&);

        s16 _0;
        s16 _2;
        s16 _4;
    };

    struct Data {
        u32 _0;
        f32 _4;
        const Point* _8;
        u32 _C;
        f32 _10;
        f32 _14;
    };
};