#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinningBox : public LiveActor {
public:
    SpinningBox(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeFall();
    void exeWait();
    void endWait();
    void exeSliding();
    void endSliding();
    void exeSpinning();
    void endSpinning();
    void exePointed();
    void endPointed();

    void calcPlanarProjectedVec(TVec3f*, const TVec3f&, const TVec3f&);
    void calcHitDirection(const TVec3f&);
    void generateIceBox(HitSensor* pSender, HitSensor* pReceiver);
    void hitReflection(HitSensor* pSender, HitSensor* pReceiver);

    f32 getScale() const {
        return 1.0f;
    }

    /* 0x8C */ TQuat4f mRotate;
    /* 0x9C */ f32 mRotateSpeed;
    /* 0xA0 */ f32 mSlideSpeed;
    /* 0xA4 */ TVec3f mSlideDir;
    /* 0xB0 */ bool mIsOnGround;
    /* 0xB1 */ bool mIsIceBox;
    /* 0xB4 */ f32 mSpinAngle;  // NOTE: This keeps track of when the spin sfx should be emitted again
};
