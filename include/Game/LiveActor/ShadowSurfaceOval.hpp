#pragma once

#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class J3DModelData;

class ShadowSurfaceOval : public ShadowSurfaceDrawer {
public:
    ShadowSurfaceOval();

    virtual ~ShadowSurfaceOval();

    virtual void draw() const override;

    void setSize(const TVec3f&);
    void setColor(Color8);
    void setAlpha(u8);

    /* 0x10 */ J3DModelData* mModelData;
    /* 0x14 */ TVec3f mSize;
    /* 0x20 */ Color8 mColor;
};
