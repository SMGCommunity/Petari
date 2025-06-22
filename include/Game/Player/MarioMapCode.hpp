#pragma once

#include <revolution.h>

class Triangle;

class FloorCode {
public:
    FloorCode();
    u32 getCode(const Triangle *);

    u32 _0;
};
