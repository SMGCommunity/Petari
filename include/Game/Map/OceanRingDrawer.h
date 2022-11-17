#pragma once

#include <revolution.h>
#include <JGeometry.h>

class OceanRing;

class OceanRingPartDrawer {
public:
    OceanRingPartDrawer(const OceanRing *, int, int, bool, f32 *, f32 *, f32 *);

    void initDisplayList(f32 *, f32 *, f32*);
    void draw() const;
    void drawGD(f32 *, f32 *, f32 *) const;
    void drawDynamic() const;
    void drawDynamicBloom() const;

    const OceanRing* mOceanRing;    // _0
    TVec3f mPosition;               // _4
    u32 _10;
    u32 _14;
    bool _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    f32 _1C;
    f32 _20;
    f32 _24;
    u32 mDispListLength;            // _28
    u8* mDispList;                  // _2C
};