#pragma once

#include "Game/Player/MarioState.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MarioActor;
class AreaObj;
class JUTTexture;

class MarioFoo : public MarioState {
public:
    MarioFoo(MarioActor*);

    virtual f32 getStickY() const;
    virtual void init();
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual void hitWall(const TVec3f&, HitSensor*);
    virtual bool passRing(const HitSensor*);
    virtual f32 getBlurOffset() const;
    virtual void draw3D() const;
    virtual const TVec3f& getGravityVec() const;

    void jet();
    void updateTilt();
    void spin();
    f32 calcRingAcc();

    /* 0x11 */ u8 _11;
    /* 0x12 */ u8 _12;
    /* 0x14 */ u32 _14;
    /* 0x18 */ u16 _18;
    /* 0x1A */ u16 _1A;
    /* 0x1C */ u16 _1C;
    /* 0x1E */ u16 _1E;
    /* 0x20 */ f32 _20;
    /* 0x24 */ f32 _24;
    /* 0x28 */ f32 _28;
    /* 0x2C */ f32 _2C;
    /* 0x30 */ TVec3f _30;
    /* 0x3C */ TVec3f _3C;
    /* 0x48 */ AreaObj* _48;
    /* 0x4C */ u16 _4C;
    /* 0x4E */ u16 _4E;
    /* 0x50 */ u16 _50;
    /* 0x54 */ f32 _54;
    /* 0x58 */ u8 _58;
    /* 0x59 */ u8 _59;
    /* 0x5A */ u16 _5A;
    /* 0x5C */ f32 _5C;
    /* 0x60 */ u16 _60;
    /* 0x64 */ TVec3f _64;
    /* 0x70 */ f32 _70;
    /* 0x74 */ f32 _74;
    /* 0x78 */ TMtx34f _78;
    /* 0xA8 */ u16 _A8;
    /* 0xAA */ u16 _AA;
    /* 0xAC */ u16 _AC;
    /* 0xAE */ u8 _AE;
    /* 0xB0 */ TVec3f _B0[64];
    /* 0x3B0 */ TVec3f _3B0[64];
    /* 0x6B0 */ u32 _6B0;
    /* 0x6B4 */ u32 _6B4;
    /* 0x6B8 */ JUTTexture* _6B8;
};
