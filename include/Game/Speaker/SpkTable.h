#pragma once

#include "revolution.h"

class SpkTable {
public:
    SpkTable();

    void setResource(void *);

    bool mInitialized;  // _0
    void* mResource;    // _4
    u32 _08;            // _8
    u32 _0C;            // _C
};