#pragma once

#include "Game/LiveActor/LiveActor.h"

class SpinDriverOperateRing : public LiveActor {
public:
    SpinDriverOperateRing(const char *);

    virtual ~SpinDriverOperateRing();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void setRadiusRate(f32);
    void reset();
    void update(const TVec3f &, const TVec3f &);
    void updatePosition();
    void updateControlPoint(const TVec3f &);
    void updateDirection(const TVec3f &);
    void resetVelocityAndTrans();
    void addAccelToOperatePlane(const TVec3f &);
    void addAccelOperate(TVec3f *, const TVec3f &);
    void addAccelToCenter();
    void attenuateVelocity();
    void calcOperatePowerByPDP(TVec3f *) const;

    TVec3f _8C;
    TVec3f _98;
    TVec3f _A4;
    TVec3f mAccelerate; // _B0
    TVec3f mDirection;  // _BC
    f32 _C8;
    f32 _CC;
    f32 _D0;
    f32 mRadiusRate;    // _D4
    f32 _D8;
    f32 _DC;
    u32 _E0;
};

namespace NrvSpinDriverOperateRing {
    NERVE(SpinDriverOperateRingNrvWait);
};