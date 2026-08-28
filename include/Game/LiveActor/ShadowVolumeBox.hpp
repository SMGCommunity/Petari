#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.hpp"
#include "Game/Util/DirectDraw.hpp"

class ShadowVolumeBox : public ShadowVolumeDrawer {
public:
    ShadowVolumeBox();

    /* 0x24 */ virtual void loadModelDrawMtx() const;
    /* 0x28 */ virtual void drawShape() const;

    void setSize(const TVec3f&);
    void makeVertexBuffer() const;

    /* 0x1C */ TVec3f mSize;
    /* 0x28 */ mutable TVec3f mPoints[14];
};
