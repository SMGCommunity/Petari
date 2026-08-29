#pragma once

#include "Game/MapObj/FloaterFloatingForce.hpp"

class SpringValue;

class FloaterSpringForce {
public:
    FloaterSpringForce(f32, f32, f32);

    void addSpringVelocity(f32);
    void setSpringBaseValue(f32);

    /* 0x00 */ f32 _0;
    /* 0x04 */ f32 _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ SpringValue* mSpringValue;
};

class FloaterFloatingForceTypeSpring : public FloaterFloatingForce {
public:
    FloaterFloatingForceTypeSpring(LiveActor*, const char*);

    virtual ~FloaterFloatingForceTypeSpring();
    virtual void init(const JMapInfoIter&);
    virtual void start();
    virtual void updateHostVelocity(TVec3f*) const;
    virtual const TVec3f& getCurrentVelocity() const {
        return mVelocity;
    }

    void updateVelocity();
    void soundMoveSE();
    f32 getCurrentSinkSpeed() const NO_INLINE;
    bool tryOn();
    bool tryReturn();
    ;
    void exeWait();
    void exeMove();
    void exeMoveReturn();

    /* 0x38 */ FloaterSpringForce* mSpringForce;
    /* 0x3C */ TVec3f mVelocity;
    /* 0x48 */ f32 _48;
};
