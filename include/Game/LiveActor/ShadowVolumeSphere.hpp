#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeSphere : public ShadowVolumeModel {
public:
    ShadowVolumeSphere();

    virtual ~ShadowVolumeSphere();
    virtual void loadModelDrawMtx() const;
    virtual bool isDraw() const;

    void setRadius(f32);

    f32 mRadius;  // 0x20
};