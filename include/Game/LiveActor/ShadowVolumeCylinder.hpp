#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeCylinder : public ShadowVolumeModel {
public:
    ShadowVolumeCylinder();

    /* 0x24 */ virtual void loadModelDrawMtx() const;

    void setRadius(f32);

    f32 mRadius;  // 0x20
};