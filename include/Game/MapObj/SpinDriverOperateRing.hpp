#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinDriverOperateRing : public LiveActor {
public:
    SpinDriverOperateRing(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void setRadiusRate(f32);
    void reset();
    void update(const TVec3f&, const TVec3f&);
    void updatePosition();
    void updateControlPoint(const TVec3f&);
    void updateDirection(const TVec3f&);
    void resetVelocityAndTrans();
    void addAccelToOperatePlane(const TVec3f&);
    void addAccelOperate(TVec3f*, const TVec3f&);
    void addAccelToCenter();
    void attenuateVelocity();
    void calcOperatePowerByDPD(TVec3f*) const;

    void exeWait();

    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ TVec3f _98;
    /* 0xA4 */ TVec3f _A4;
    /* 0xB0 */ TVec3f mAccelerate;
    /* 0xBC */ TVec3f mDirection;
    /* 0xC8 */ TVec3f _C8;
    /* 0xD4 */ f32 mRadiusRate;
    /* 0xD8 */ f32 _D8;
    /* 0xDC */ f32 _DC;
    /* 0xE0 */ s32 _E0;
};
