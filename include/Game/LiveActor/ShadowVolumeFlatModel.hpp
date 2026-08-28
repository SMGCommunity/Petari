#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.hpp"

class J3DModelData;

class ShadowVolumeFlatModel : public ShadowVolumeDrawer {
public:
    ShadowVolumeFlatModel(const char*);
    ShadowVolumeFlatModel();

    inline virtual ~ShadowVolumeFlatModel();
    /* 0x18 */ virtual void draw() const;

    void initModel(const char*);
    void setBaseMatrixPtr(MtxPtr);
};
