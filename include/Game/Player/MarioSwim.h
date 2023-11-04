#pragma once

#include "Game/Player/MarioState.h"
#include "Game/Player/WaterInfo.h"

class MarioActor;
class MarineSnow;

class MarioSwim : public MarioState {
public:
    MarioSwim(MarioActor *);

    virtual void init();
    virtual bool proc(u32);
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool keep();
    virtual bool postureCtrl(MtxPtr);
    virtual void hitWall(const TVec3f &, HitSensor *);
    virtual void hitPoly(u8, const TVec3f &, HitSensor *);
    virtual bool passRing(const HitSensor *);
    virtual f32 getBlurOffset() const;
    virtual void draw3D() const;
    
    void checkWaterCube(bool);

    virtual TVec3f& getGravityVec() const;

    MarineSnow* _14;
    u8 _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    u8 _1C;
    u8 _1D;
    u8 _1E;
    u8 _1F;
    u8 _20;
    u8 _21;
    u8 _22;
    u32 _24;
    u32 _28;
    u16 _2C;
    u16 _2E;
    u16 _30;
    u16 _32;
    u16 _34;
    u16 _36;
    u16 _38;
    u16 _3A;
    u16 _3C;
    u16 _3E;
    u16 _40;
    u16 _42;
    u16 _44;
    f32 _48;
    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    f32 _70;
    f32 _74;
    u8 _78;
    u16 _7A;
    u16 _7C;
    f32 _80;
    f32 _84;
    u8 _88;
    u16 _8A;
    u16 _8C;
    u16 _8E;
    u16 _90;
    f32 _94;
    f32 _98;
    u8 _9C;
    u8 _9D;
    u8 _9E;
    u8 _9F;
    TVec3f _A0;
    u8 _AC;
    u8 _AD;
    u16 _AE;
    TMtx34f _B0;
    f32 _E0;
    f32 _E4;
    u16 _E8;
    u16 _EA;
    u16 _EC;
    u16 _EE;
    u16 _F0;
    WaterInfo _F4;
    u32 _144;
    TVec3f _148;
    TVec3f _154;
    TVec3f _160;
    TVec3f _16C;
    TVec3f _178;
    TVec3f _184;
    TVec3f _190;
    f32 _19C;
    f32 _1A0;
    f32 _1A4;
    f32 _1A8;
    f32 _1AC;
    u16 _1B0;
    u8 _1B2;
    f32 _1B4;
    f32 _1B8;
};
