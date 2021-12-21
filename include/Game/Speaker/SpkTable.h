#pragma once

#include "revolution.h"

class SpkTable {
public:
    SpkTable();

    void setResource(void* resource);

    bool mInitialized;
    void* mResource;
    u32 _08;
    u32 _0C;
};