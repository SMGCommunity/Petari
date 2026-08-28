#pragma once

#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ShadowSurfaceOval : public ShadowSurfaceDrawer {
public:
    ShadowSurfaceOval();

    /* 0x08 */ virtual ~ShadowSurfaceOval();

    /* 0x18 */ virtual void draw() const override;

    void setSize(const TVec3f&);
    void setColor(Color8);
    void setAlpha(u8);
};
