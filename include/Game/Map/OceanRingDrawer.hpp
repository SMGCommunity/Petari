#pragma once

#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution.h>

class OceanRing;

class OceanRingPartDrawer {
public:
    OceanRingPartDrawer(const OceanRing*, int, int, bool, f32*, f32*, f32*);

    void initDisplayList(f32*, f32*, f32*);
    void draw() const;
    void drawGD(f32*, f32*, f32*) const;
    void drawDynamic() const;
    void drawDynamicBloom() const;

    const OceanRing* mOceanRing;  // 0x0
    TVec3f mPosition;             // 0x4
    int _10;
    int _14;
    bool _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    f32 _1C;
    f32 _20;
    f32 _24;
    u32 mDispListLength;  // 0x28
    u8* mDispList;        // 0x2C
};

class OceanRingDrawer {
public:
    OceanRingDrawer(const OceanRing*);

    void update();
    void draw() const;
    void drawBloom() const;
    void initParts();
    void initDisplayList();
    void drawGD() const;
    void loadMaterial() const;
    void loadMaterialBloom() const;

    OceanRingPartDrawer* getDrawer(int idx) const { return mPartDrawers[idx]; }

    inline f32 someInline(f32 a1, f32 a2, f32 a3) const { return (a1 - a2) / a3; }

    const OceanRing* mRing;              // 0x0
    s32 mDrawerCount;                    // 0x4
    OceanRingPartDrawer** mPartDrawers;  // 0x8
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    JUTTexture* mWaterTex;     // 0x24
    JUTTexture* mWaterIndTex;  // 0x28
    u32 _2C;
    u8* mDispList;  // 0x30
};