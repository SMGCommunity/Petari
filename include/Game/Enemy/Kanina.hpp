#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimationRandomPlayer;
class BenefitItemOneUp;
class JointRumbler;

class Kanina : public LiveActor {
public:
    enum KaninaType {
        KaninaType_Blue = 0,
        KaninaType_Red = 1,
    };

    Kanina(const char*);

    virtual ~Kanina();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void appearItemPunch();
    void appearItemTrample();
    bool tryAttack(HitSensor*, HitSensor*);
    bool tryPushEach(HitSensor*, HitSensor*);
    bool receiveTrample(HitSensor*, HitSensor*);
    bool receivePunch(HitSensor*, HitSensor*);
    bool receiveFireBall(HitSensor*, HitSensor*);
    void doDamageFireBall(HitSensor*, HitSensor*);
    void startRun();
    void startRunAwayLevelSound();
    bool receiveHipDrop(HitSensor*, HitSensor*);
    bool receiveInvincibleAttack(HitSensor*, HitSensor*);
    bool isStateStayOnGround() const;
    void initForType(const JMapInfoIter&, KaninaType);
    bool isPlayerBackward(f32) const;
    bool isStatePossibleToAttack() const;
    bool isStateBlink() const;
    bool isStateGuard() const;
    bool isStateDamageNoFireBall() const;
    bool isBindedGroundWaterBottom() const;
    void updateMovement();
    bool tryFindPlayer();
    bool tryHitWall();
    bool tryTurn();
    bool tryPointing();

    void exeAppear();
    void exeWait();
    void exeAttack();
    void exeVauntAttackSuccess();
    void exeGuard();
    void exeGuardEnd();
    void exeDamageFireBall();
    void exeDamageHipDrop();
    void exeDig();
    void exeWaitUnderGround();
    void exeFindPlayer();
    void exeRunAwayBreak();
    void exeReboundEach();
    void exeTurn();
    void exeTurnEnd();
    void exePointing();
    void endPointing();
    void exeWalk();
    void exeRunAway();
    void exeRunAwayReboundDirection();
    void exeHitWall();

    inline f32 getFloatDependentType(f32 fRed, f32 fBlue) {
        return mType == KaninaType_Red ? fRed : fBlue;
    }

    /* 0x8C */ JointRumbler* mJointRumbler;
    /* 0x90 */ BenefitItemOneUp* mKinokoOneUp;
    /* 0x94 */ AnimationRandomPlayer* mAnimationRandomPlayer;
    /* 0x98 */ TVec3f _98;
    /* 0xA4 */ TQuat4f _A4;
    /* 0xB4 */ u32 _B4;
    /* 0xB8 */ u32 _B8;
    /* 0xBC */ f32 _BC;
    /* 0xC0 */ TVec3f _C0;
    /* 0xCC */ KaninaType mType;
    /* 0xD0 */ TVec3f _D0;
    /* 0xDC */ s32 _DC;
};
