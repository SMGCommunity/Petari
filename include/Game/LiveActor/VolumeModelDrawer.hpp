#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include <revolution.h>

class VolumeModelDrawInit : public NameObj {
public:
    VolumeModelDrawInit();

    virtual ~VolumeModelDrawInit();
};

class VolumeModelDrawer : public NameObj {
public:
    VolumeModelDrawer(const char*, const char*, MtxPtr);

    virtual ~VolumeModelDrawer();
    virtual void draw() const;

    void loadModelDrawMtx() const;

    MtxPtr mMtx;               // 0xC
    J3DModelData* mModelData;  // 0x10
    _GXColor mColor;           // 0x14
};