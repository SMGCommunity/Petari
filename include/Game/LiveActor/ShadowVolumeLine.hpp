#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.hpp"

class ShadowVolumeLine : public ShadowVolumeDrawer {
public:
    ShadowVolumeLine();

    /* 0x08 */ virtual ~ShadowVolumeLine();
    /* 0x24 */ virtual void loadModelDrawMtx() const;
    /* 0x28 */ virtual void drawShape() const;

    void setFromShadowController(const ShadowController*);
    void setToShadowController(const ShadowController*);
    void setFromWidth(f32);
    void setToWidth(f32);

    const ShadowController* mFromShadowController;  // 0x1C
    const ShadowController* mToShadowController;    // 0x20
    f32 mFromWidth;                                 // 0x24
    f32 mToWidth;                                   // 0x28
};