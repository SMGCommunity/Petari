#pragma once

class J3DMaterialAnm;
class J3DModelData;
class ResourceHolder;

class MaterialAnmBuffer {
public:
    MaterialAnmBuffer(const ResourceHolder*, J3DModelData*, bool);

    /* 0x00 */ J3DMaterialAnm* _0;
    /* 0x04 */ void* _4;
};
