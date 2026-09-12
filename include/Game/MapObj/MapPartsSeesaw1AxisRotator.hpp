#pragma once

#include "Game/MapObj/MapPartsRotator.hpp"

class MapPartsSeesaw1AxisRotator : public MapPartsRotatorBase {
public:
    MapPartsSeesaw1AxisRotator(LiveActor*, const char*, f32);

    virtual ~MapPartsSeesaw1AxisRotator();
    virtual void init(const JMapInfoIter&) override;
    virtual bool isWorking() const override;
    virtual void start() override;
    virtual void end() override;
    virtual bool receiveMsg(u32) override;
    virtual const TMtx34f& getRotateMtx() const override {
        return mRotateMtx;
    }

    virtual bool isMoving() const override {
        return isWorking();
    }

    void exeWait() {
    }

    void exeMove();
    void exeStay();
    void exeHipDrop();
    void rotate();
    bool isAngleLimited() const {
        return 0.0f < mRotateAngle;
    }

    void updateVelocity();
    void updateRestoreForce();
    void clampAngularSpeed();
    f32 getDistanceFromRotAxis() const;
    void addForceHipDrop();
    bool isGoingToReachTargetAngle() const;
    void calcRotatedAngle(f32*, const TPos3f&) const;
    bool tryHipDrop();

    /* 0x18 */ f32 mAngularSpeedMax;
    /* 0x1C */ f32 mInertiaConst;
    /* 0x20 */ f32 mRotateAngle;
    /* 0x24 */ f32 mRestoreForce;
    /* 0x28 */ TPos3f mRotateMtx;
    /* 0x58 */ bool mHipDrop;
    /* 0x5C */ TVec3f mRotateAxis;
    /* 0x68 */ f32 mAngularVelocity;
    /* 0x6C */ f32 mAngularAccel;
    /* 0x70 */ TVec3f mInitialUp;
    /* 0x7C */ const char* mMoveSound;
    /* 0x80 */ f32 mMoveSoundSpeed;
};
