#pragma once

#include "Game/LiveActor/ShadowVolumeModel.h"

class ShadowVolumeOval : public ShadowVolumeModel {
public:
    ShadowVolumeOval();

    virtual bool isDraw() const;

    void setSize(const TVec3f &);

    TVec3f mSize;   // _20
};