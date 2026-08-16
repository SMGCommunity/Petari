#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class Gesso : public LiveActor {
public:
    Gesso(const char*);

    virtual ~Gesso();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void selectNextNerve();
    void knockOut(HitSensor*, HitSensor*);
    bool clipAndInitPos();
    bool calcWalkMove(s32);
    bool calcSinkMove(s32);
    void calcAndSetVelocity(f32, f32, f32);
    void calcAndSetUpVecTarget(f32, f32, f32);
    bool isMarioUp() const;
    bool isMarioLeft() const NO_INLINE;
    bool tryChangeHighSpeedMode();
    bool tryDPDSwoon();

    void exeComeFromBox();
    void exeWait();
    void exeSearch();
    void exeWalkCharge();
    void exeWalk();
    void exeSink();
    void exeLostPlayer();
    void exeAttack();
    void exePunchDown();
    void exeComeBack();
    void exeDPDSwoon();
    void endDPDSwoon();
    void exeRotate();

private:
    /* 0x8C */ AnimScaleController* mScaleController;
    /* 0x90 */ WalkerStateBindStarPointer* mStateBindStarPointer;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ TVec3f _AC;
    /* 0xB8 */ TVec3f _B8;
    /* 0xC4 */ s32 _C4;
    /* 0xC8 */ TVec3f _C8;
    /* 0xD4 */ bool mIsMarioLeft;
    /* 0xD5 */ bool mIsHighSpeedMode;
};
