#pragma once

#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowDrawer.hpp"
#include "Game/Util/Color.hpp"

class ShadowVolumeDrawInit : public NameObj {
public:
    ShadowVolumeDrawInit();

    virtual ~ShadowVolumeDrawInit();
};

class ShadowVolumeDrawer : public ShadowDrawer {
public:
    ShadowVolumeDrawer(const char*);

    virtual ~ShadowVolumeDrawer();

    virtual void draw() const override;
    virtual void loadModelDrawMtx() const override;
    virtual void drawShape() const override;
    virtual bool isDraw() const;

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
