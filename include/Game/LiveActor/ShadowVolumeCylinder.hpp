#pragma once

#include "Game/LiveActor/ShadowVolumeModel.h"

class ShadowVolumeCylinder : public ShadowVolumeModel {
public:
    ShadowVolumeCylinder();

    virtual void loadModelDrawMtx() const;

    void setRadius(f32);

    f32 mRadius;    // _20
};