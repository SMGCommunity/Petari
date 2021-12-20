#pragma once

#include "revolution.h"

class SpkTable {
public:
    SpkTable();

    void setResource(void* resource);

    bool initialized;
    void *resource;
    u32 unk_1;
    u32 unk_2;
};