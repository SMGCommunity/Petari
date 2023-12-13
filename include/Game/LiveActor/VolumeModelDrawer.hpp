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
    VolumeModelDrawer(const char *, const char *, MtxPtr);

    virtual ~VolumeModelDrawer();
    virtual void draw() const;

    void loadModelDrawMtx() const;


    MtxPtr mMtx;                // _C
    J3DModelData* mModelData;   // _10
    _GXColor mColor;             // _14
};