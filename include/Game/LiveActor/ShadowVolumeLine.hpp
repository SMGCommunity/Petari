#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.hpp"

class ShadowVolumeLine : public ShadowVolumeDrawer {
public:
    ShadowVolumeLine();

    virtual ~ShadowVolumeLine();
    virtual void loadModelDrawMtx() const;
    virtual void drawShape() const;

    void setFromShadowController(const ShadowController *);
    void setToShadowController(const ShadowController *);
    void setFromWidth(f32);
    void setToWidth(f32);

    const ShadowController* mFromShadowController;    // _1C
    const ShadowController* mToShadowController;      // _20
    f32 mFromWidth;                             // _24
    f32 mToWidth;                               // _28
};