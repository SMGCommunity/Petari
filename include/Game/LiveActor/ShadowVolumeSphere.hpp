#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeSphere : public ShadowVolumeModel {
public:
    ShadowVolumeSphere();

    /* 0x08 */ virtual ~ShadowVolumeSphere();
    /* 0x24 */ virtual void loadModelDrawMtx() const;
    /* 0x2C */ virtual bool isDraw() const;

    void setRadius(f32);

    f32 mRadius;  // 0x20
};