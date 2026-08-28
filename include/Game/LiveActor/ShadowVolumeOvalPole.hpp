#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeOvalPole : public ShadowVolumeModel {
public:
    ShadowVolumeOvalPole();

    /* 0x08 */ virtual ~ShadowVolumeOvalPole();
    /* 0x24 */ virtual void loadModelDrawMtx() const;

    void setSize(const TVec3f&);

    TVec3f mSize;  // 0x20
};