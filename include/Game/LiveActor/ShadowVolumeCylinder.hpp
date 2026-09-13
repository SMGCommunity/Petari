#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeCylinder : public ShadowVolumeModel {
public:
    ShadowVolumeCylinder();

    virtual ~ShadowVolumeCylinder();
    virtual void loadModelDrawMtx() const;

    void setRadius(f32);

    /* 0x20 */ f32 mRadius;
};