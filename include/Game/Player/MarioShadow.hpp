#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class J3DModelX;
class Triangle;
class JUTTexture;

class CollisionShadow : public NameObj {
public:
    CollisionShadow(f32, f32);

    virtual ~CollisionShadow();
    virtual void draw() const;

    void setMode(u32);
    void create(const TVec3f&, const TVec3f&, const TVec3f&);
    void calcView(J3DModelX*, u32, J3DModelX*);
    void drawAndCaptureTex(J3DModelX*, const TVec3f&);
    void clearAlphaBuffer();
    void setUpdateFlag();
    void draw1() const;
    void sendVtx() const;
    void sendZsortedVtx(bool) const;
    void initCaptureTex();
    void setViewMtx(const TVec3f&);
    void drawVolumeBox(const TVec3f&, const TVec3f&) const;
    void draw2() const;
    void draw3() const;
    void createDL();
    void drawVolume() const;
    void doSortPolygons();
    const TVec3f& getDrawPos(u32) const;
    const TVec3f& getDrawPos(u32, u32) const;
    const TVec2f& getDrawTx(u32, u32) const;

    /* 0x00C */ u8 _C;
    /* 0x00E */ u16 _E;
    /* 0x010 */ f32 _10;
    /* 0x014 */ f32 _14;
    /* 0x018 */ f32 _18;
    /* 0x01C */ f32 _1C;
    /* 0x020 */ f32 _20;
    /* 0x024 */ TVec3f _24;
    /* 0x030 */ TVec3f _30;
    /* 0x03C */ TVec3f _3C;
    /* 0x048 */ TVec3f _48;
    /* 0x054 */ u32 _54;
    /* 0x058 */ u16 _58;
    /* 0x05A */ u16 _5A;
    /* 0x05C */ u8* _5C;
    /* 0x060 */ TVec3f* _60;
    /* 0x064 */ TVec2f* _64;
    /* 0x068 */ u16* _68;
    /* 0x06C */ u16 _6C;
    /* 0x06E */ u16 _6E;
    /* 0x070 */ u8 _70[128];
    /* 0x0F0 */ s32 _F0[128];
    /* 0x2F0 */ TVec3f _2F0;
    /* 0x2FC */ u16 _2FC;
    /* 0x2FE */ u16 _2FE;
    /* 0x300 */ JUTTexture* _300;
    /* 0x304 */ u8 _304;
    /* 0x305 */ bool _305;
    /* 0x306 */ u8 _306;
    /* 0x307 */ u8 _307;
    /* 0x308 */ u16 _308;
    /* 0x30C */ f32 _30C;
    /* 0x310 */ f32 _310;
    /* 0x314 */ TVec3f _314;
    /* 0x320 */ TVec3f _320;
    /* 0x32C */ Triangle* _32C;
    /* 0x330 */ Triangle* _330;
    /* 0x334 */ Triangle* _334;
    /* 0x338 */ u16 _338;
    /* 0x33A */ u16 _33A;
    /* 0x33C */ u16 _33C;
    /* 0x340 */ J3DModelX* _340;
    /* 0x344 */ u8* _344;
    /* 0x348 */ u16 _348;
};
