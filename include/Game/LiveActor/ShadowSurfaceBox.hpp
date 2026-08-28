#pragma once

#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ShadowSurfaceBox : public ShadowSurfaceDrawer {
public:
    ShadowSurfaceBox();

    /* 0x08 */ virtual ~ShadowSurfaceBox();

    /* 0x18 */ virtual void draw() const override;

    void setSize(const TVec3f&);
};
