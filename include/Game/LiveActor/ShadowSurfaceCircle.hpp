#pragma once

#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"

class ShadowSurfaceCircle : public ShadowSurfaceDrawer {
public:
    ShadowSurfaceCircle();

    /* 0x08 */ virtual ~ShadowSurfaceCircle();

    /* 0x18 */ virtual void draw() const override;

    void setRadius(f32);

    f32 _10;
};
