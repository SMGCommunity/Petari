#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;
class HitSensor;
class Triangle;

class MarioHang : public MarioState {
public:
    MarioHang(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool postureCtrl(MtxPtr);

    void recordWallPolygon(const Triangle*);
    void recordHangNorm(const TVec3f&);
    void forceDrop();
    void tryClimb(bool);

    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 _14;
    /* 0x16 */ u16 _16;
    /* 0x18 */ u16 mHangTimer;
    /* 0x1A */ u8 _1A;
    /* 0x1B */ u8 _1B;
    /* 0x1C */ u8 _1C;
    /* 0x1D */ u8 _1D;
    /* 0x1E */ u8 _1E;
    /* 0x1F */ u8 _1F;
    /* 0x20 */ Triangle* _20;
    /* 0x24 */ Triangle* _24;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ TVec3f _34;
    /* 0x40 */ HitSensor* mWallSensor;
};
