#pragma once

#include <revolution.h>

class ResourceHolder;
class J3DModelData;

class MaterialAnmBuffer {
public:
    MaterialAnmBuffer(const ResourceHolder*, J3DModelData*, bool);

    /* 0x00 */ u32 _0;
    /* 0x04 */ u32 _4;
};
