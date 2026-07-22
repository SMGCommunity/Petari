#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include <revolution.h>

class VolumeModelDrawer : public NameObj {
public:
    VolumeModelDrawer(const char*, const char*, MtxPtr);

    virtual void draw() const;

    void loadModelDrawMtx() const NO_INLINE;

    /* 0x0C */ MtxPtr mMtx;
    /* 0x10 */ J3DModelData* mModelData;
    /* 0x14 */ GXColor mColor;
};

class VolumeModelDrawInit : public NameObj {
public:
    VolumeModelDrawInit();
};
