#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FixedPosition;

class Karikari : public LiveActor {
public:
    Karikari(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void applyEnvironmentInfluenceToVelocity();
    void killedInFrozenState();

    void exeFall();
    void exeLand();
    void exeNoCalcWait();
    void exeWait();
    void exePrePursue();
    void exePursue();
    void exeWatchFor();
    void exePreCling();
    void exeCling();
    void exeRelease();
    void exeSpinAttacked();
    void exeDPDAttacked();
    void exeFrozen();
    void exeFrozenRecover();
    void exeBlowOut();
    void exeInTornado();
    void exePress();

    bool tryBlowOut(const TVec3f&, bool);
    bool tryElectricKill();
    bool tryHipDropRelease();
    bool tryDPDRelease(const TVec2f&);
    bool tryDPDAttacked();

    void setVelocityFromCursorMove(const TVec2f&);
    void generateItem(s32 numStarPieces);

    void tryTurnToDirection(const TVec3f& rDir, f32 turnRatio);

    bool inClingNerve() const;

    /* 0x8C */ TVec3f mFront;
    /* 0x98 */ FixedPosition* mClingPosition;
    /* 0x9C */ bool mIsReadyToLandTornado;
    /* 0x9D */ bool _9D;
    /* 0x9E */ bool _9E;
    /* 0x9F */ bool mIsPushable;
    /* 0xA0 */ s32 mFrozenTime;
    /* 0xA4 */ u32 _A4;
};
