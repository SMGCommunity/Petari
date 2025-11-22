#pragma once

#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include "Game/Util.hpp"
#include "Game/Util/Color.hpp"

class ShadowSurfaceOval : public ShadowSurfaceDrawer {
public:
    ShadowSurfaceOval();

    virtual ~ShadowSurfaceOval();

    virtual void draw() const;

    void setSize(const TVec3f&);
    void setColor(Color8);
    void setAlpha(u8);
};
