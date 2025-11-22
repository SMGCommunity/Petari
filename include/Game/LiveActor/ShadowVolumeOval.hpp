#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeOval : public ShadowVolumeModel {
public:
    ShadowVolumeOval();

    virtual bool isDraw() const;

    void setSize(const TVec3f&);

    TVec3f mSize;  // 0x20
};