#pragma once

#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ShadowSurfaceBox : public ShadowSurfaceDrawer {
public:
    ShadowSurfaceBox();

    virtual ~ShadowSurfaceBox();

    virtual void draw() const override;

    void setSize(const TVec3f&);
};
