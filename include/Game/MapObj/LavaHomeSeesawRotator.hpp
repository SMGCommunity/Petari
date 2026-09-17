#pragma once

#include "Game/MapObj/MapPartsRotator.hpp"

class LavaHomeSeesawRotator : public MapPartsRotatorBase {
public:
    LavaHomeSeesawRotator(LiveActor*, const char*, f32);

    virtual ~LavaHomeSeesawRotator();
    virtual void init(const JMapInfoIter&);
    virtual bool isWorking() const {
        return true;
    }

    virtual void start();
    virtual void end();
    virtual bool receiveMsg(u32);
    virtual const TMtx34f& getRotateMtx() const {
        return mRotateMtx;
    }

    void exeWait();
    void exeMoveStart();
    void exeMove();
    void exeStay();
    void exeHipDrop();
    void updateRotateMtx();
    void updateVelocity();
    void updateRestoreForce();
    void clampAngularSpeed();
    f32 getDistanceFromRotAxis() const;
    void addForceHipDrop();
    bool isAngleLimited() const;
    bool isGoingToReachTargetAngle() const;
    void calcRotatedAngle(f32*, const TPos3f&) const;
    bool tryHipDrop();
    bool tryStay();
    bool tryRebound();
    void soundMoveSE();

    /* 0x18 */ f32 mAngularSpeedMax;
    /* 0x1C */ f32 mInertia;
    /* 0x20 */ f32 mRotateAngle;
    /* 0x24 */ f32 mRestoreForce;
    /* 0x28 */ TPos3f mRotateMtx;
    /* 0x58 */ TQuat4f mRotateQuat;
    /* 0x68 */ bool mIsHipDropped;
    /* 0x6C */ TVec3f mRotateAxis;
    /* 0x78 */ f32 mAngularVelocity;
    /* 0x7C */ TVec3f mInitialUp;
    /* 0x88 */ const char* mMoveSound;
    /* 0x8C */ f32 mMoveSoundSpeed;
};
