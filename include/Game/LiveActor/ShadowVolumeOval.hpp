#pragma once

#include "Game/LiveActor/ShadowVolumeModel.hpp"

class ShadowVolumeOval : public ShadowVolumeModel {
public:
    ShadowVolumeOval();

    /* 0x2C */ virtual bool isDraw() const;

    void setSize(const TVec3f&);

    TVec3f mSize;  // 0x20
};