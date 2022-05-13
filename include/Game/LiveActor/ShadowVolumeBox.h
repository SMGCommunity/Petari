#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.h"
#include "Game/Util/DirectDraw.h"

class ShadowVolumeBox : public ShadowVolumeDrawer {
public:
    ShadowVolumeBox();

    virtual ~ShadowVolumeBox();

    virtual void draw() const;
    virtual void loadModelDrawMtx() const;
    virtual void drawShape() const;

    void setSize(const TVec3f &);
    void makeVertexBuffer() const;

    TVec3f mSize;   // _1C
    TVec3f _28;
    TVec3f _34;
    TVec3f _40;
    TVec3f _4C;
    TVec3f _58;
    TVec3f _64;
    TVec3f _70;
    TVec3f _7C;
    TVec3f _88;
    TVec3f _94;
    TVec3f _A0;
    TVec3f _AC;
    TVec3f _B8;
    TVec3f _C4;
};