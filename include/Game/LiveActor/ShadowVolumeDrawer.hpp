#pragma once

#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowDrawer.hpp"
#include "Game/Util/Color.hpp"

class ShadowVolumeDrawInit : public NameObj {
public:
    ShadowVolumeDrawInit();

    /* 0x08 */ virtual ~ShadowVolumeDrawInit();
};

class ShadowVolumeDrawer : public ShadowDrawer {
public:
    ShadowVolumeDrawer(const char*);

    /* 0x08 */ virtual ~ShadowVolumeDrawer();

    /* 0x18 */ virtual void draw() const override;
    /* 0x24 */ virtual void loadModelDrawMtx() const override;
    /* 0x28 */ virtual void drawShape() const override;
    /* 0x2C */ virtual bool isDraw() const;

    void setStartDrawShepeOffset(f32);
    void setEndDrawShepeOffset(f32);
    void onCutDropShadow();
    void offCutDropShadow();
    void calcBaseDropPosition(TVec3f*) const;
    void calcBaseDropPosition(TVec3f*, const ShadowController*) const;
    f32 calcBaseDropLength() const;
    f32 calcBaseDropLength(const ShadowController*) const;

    f32 mStartDrawShapeOffset;  // 0x10
    f32 mEndDrawShapeOffset;    // 0x14
    bool mIsCutDropShadow;      // 0x18
};
