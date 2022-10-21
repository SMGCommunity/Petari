#pragma once

#include "Game/LiveActor/ShadowDrawer.h"
#include "Game/LiveActor/ShadowController.h"
#include "Game/Util/Color.h"

class ShadowVolumeDrawInit : public NameObj {
public:
    ShadowVolumeDrawInit();

    virtual ~ShadowVolumeDrawInit();
};

class ShadowVolumeDrawer : public ShadowDrawer {
public:
    ShadowVolumeDrawer(const char *);

    virtual ~ShadowVolumeDrawer();

    virtual void draw() const;
    virtual void loadModelDrawMtx() const;
    virtual void drawShape() const;
    virtual bool isDraw() const;

    void setStartDrawShepeOffset(f32);
    void setEndDrawShepeOffset(f32);
    void onCutDropShadow();
    void offCutDropShadow();
    void calcBaseDropPosition(TVec3f *) const;
    void calcBaseDropPosition(TVec3f *, const ShadowController *) const;
    f32 calcBaseDropLength() const;
    f32 calcBaseDropLength(const ShadowController *) const;

    f32 mStartDrawShapeOffset;      // _10
    f32 mEndDrawShapeOffset;        // _14
    bool mIsCutDropShadow;          // _18
};