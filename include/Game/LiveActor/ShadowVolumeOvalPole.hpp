#pragma once

#include "Game/LiveActor/ShadowVolumeModel.h"

class ShadowVolumeOvalPole : public ShadowVolumeModel {
public:
    ShadowVolumeOvalPole();

    virtual ~ShadowVolumeOvalPole();
    virtual void loadModelDrawMtx() const;

    void setSize(const TVec3f &);

    TVec3f mSize;   // _20
};