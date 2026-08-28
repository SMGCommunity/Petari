#pragma once

#include "Game/MapObj/FloaterFloatingForce.hpp"

class FloaterFloatingForceTypeNormal : public FloaterFloatingForce {
public:
    FloaterFloatingForceTypeNormal(LiveActor*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void start();
    /* 0x38 */ virtual void updateHostTrans(TVec3f*) const;
    /* 0x40 */ virtual const TVec3f& getCurrentVelocity() const {
        return mVelocity;
    }

    void updateVelocity(bool);
    void soundMoveSE();
    void setStateMove();
    void exeWait();
    void exeMoveFirst();
    void exeMove();

    /* 0x38 */ f32 mRotateAngle;
    /* 0x3C */ f32 mRotateSpeed;
    /* 0x40 */ f32 _40;
    /* 0x44 */ f32 _44;
    /* 0x48 */ TVec3f mVelocity;
};
