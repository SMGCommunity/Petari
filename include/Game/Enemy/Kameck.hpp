#pragma once

#include "Game/Enemy/KameckBeam.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;
class ActiveActorList;
class Kameck;

class SmallKameckBeamEventListener : public KameckBeamEventListener {
public:
    SmallKameckBeamEventListener(Kameck* kameck) : mHost(kameck) {
    }

    virtual void hitBeam(s32);

    Kameck* mHost;
};

class Kameck : public LiveActor {
public:
    enum BeamType {
        /* 0x0 */ BeamType_None,
        /* 0x1 */ BeamType_Turtle,
        /* 0x2 */ BeamType_Fire,
        /* 0x3 */ BeamType_3,
        /* 0x4 */ BeamType_4
    };

    Kameck(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorDead();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void startClipped();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initBeam();
    void initJMapParam(const JMapInfoIter& rIter);
    void initDemo(const JMapInfoIter& rIter);
    void exeWait();
    void exeAppear();
    void exeHit();
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeDown();
    void exeOpeningDemo();
    void exeDemoAppear();
    void endDemoAppear();
    void exeNonActive();
    void exeHide();
    void exeAttackWait();
    void exeAttack();
    void exeGuard();
    void exeMoveHide();
    void exeMove();
    void exePressDown();
    void startDemoAppear();
    void killForce();
    void resetBeam();
    bool isEnableAttack() const;
    bool isEnableDown() const;
    bool isEnableGurad() const;
    bool tryNonActive();
    bool requestAttack(HitSensor*, HitSensor*);
    bool requestGuard(HitSensor*, HitSensor*);
    bool requestDown(HitSensor*, HitSensor*);
    bool requestPressDown();
    bool tryOpeningDemo();
    bool tryOpeningDemoEnd();
    bool canNonActive() const;
    void setNonActive();
    bool tryActive();
    bool tryAppearEnd();
    bool tryAttackWait();
    bool tryAttack();
    bool tryAttackEnd();
    bool tryHitEnd();
    bool tryMove();
    bool tryMoveEnd();
    bool tryPointBind();
    void hitBeam(s32);
    void setBeamType(s32);
    void makeActorDeadForce();

    /* 0x8C */ KameckBeam* mBeam;
    /* 0x90 */ ActiveActorList* _90;
    /* 0x94 */ SmallKameckBeamEventListener* _94;
    /* 0x98 */ AnimScaleController* _98;
    /* 0x9C */ WalkerStateBindStarPointer* _9C;
    /* 0xA0 */ TQuat4f _A0;
    /* 0xB0 */ TVec3f _B0;
    /* 0xBC */ s32 mBeamType;
    /* 0xC0 */ s32 mMoveStep;
    /* 0xC4 */ f32 mRailCoord;
    /* 0xC8 */ f32 mRailNextPointCoord;
    /* 0xCC */ f32 mActiveDistance;
};

namespace MR {
    NameObj* createFireBallBeamKameck(const char*);
};  // namespace MR
