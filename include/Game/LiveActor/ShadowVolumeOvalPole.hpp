#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeOvalPole : public ShadowVolumeModel {
public:
    ShadowVolumeOvalPole();

    virtual ~ShadowVolumeOvalPole();
    virtual void loadModelDrawMtx() const;

    void setSize(const TVec3f&);

    TVec3f mSize;  // 0x20
};