#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FloaterFloatingForce.hpp"

class FloaterFloatingForceTypeNormal : public FloaterFloatingForce {
public:
    FloaterFloatingForceTypeNormal(LiveActor*, const char*);

    virtual ~FloaterFloatingForceTypeNormal();
    virtual void init(const JMapInfoIter&);
    virtual void start();
    virtual void updateHostTrans(TVec3f*) const;
    virtual const TVec3f& getCurrentVelocity() const;

    void updateVelocity(bool);
    void soundMoveSE();
    void setStateMove();
    void exeWait();
    void exeMoveFirst();
    void exeMove();

    f32 mRotateAngle;  // 0x38
    f32 mRotateSpeed;  // 0x3C
    f32 _40;
    f32 _44;
    TVec3f mVelocity;  // 0x48
};
