#pragma once

#include "revolution.h"

class JAISoundID {
public:
    JAISoundID() {

    };
    JAISoundID(u32 id) : mID(id) {}
    JAISoundID(JAISoundID const& other) { mID = other.mID; }

    /* 0x0 */ u32 mID;
};
