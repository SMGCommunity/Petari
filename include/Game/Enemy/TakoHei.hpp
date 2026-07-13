#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class HitSensor;
class WalkerStateBindStarPointer;

class TakoHei : public LiveActor {
public:
    TakoHei(const char*);

    virtual ~TakoHei();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initSensor();

    bool requestPressDown();
    bool requestHipDropDown();
    bool requestFlatDown();
    bool requestSwoon();
    bool requestPunch(HitSensor*, HitSensor*);
    bool requestAttackSuccess(HitSensor*, HitSensor*);

    bool tryActive();
    bool tryNonActive();
    bool tryWalk();
    bool tryWalkEnd();
    bool tryFindTurn();
    bool tryFind();
    bool tryPursue();
    bool tryPursueEnd();
    bool tryAttack();
    bool tryCoolDownEnd();
    bool tryAttackSuccessEnd();
    bool tryPointBind();
    bool tryRecover();
    bool tryRecoverEnd();
    bool tryPressed();

    void exeNonActive();
    void exeWait();
    void exeWalk();
    void exeFindTurn();
    void exeFind();
    void exePursue();
    void exeCoolDown();
    void exeAttackSign();
    void exeAttack();
    void exeAttackSuccess();
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeSwoon();
    void exeRecover();
    void exePressDown();
    void exeHipDropDown();
    void exeFlatDown();
    void exePunchDown();

    void updatePose();
    void updateNormalVelocity();
    void updateSwoonVelocity();
    void decideNextTargetPos();

    bool checkFront() const;
    bool canAttack() const;
    bool isPushMovable() const;
    bool isEnableKick() const;
    bool isDown() const;
    bool isFallNextMove() const;
    bool isInSightMario() const;

    /* 0x8C */ AnimScaleController* _8C;
    /* 0x90 */ WalkerStateBindStarPointer* _90;
    /* 0x94 */ TQuat4f _94;
    /* 0xA4 */ TVec3f _A4;
    /* 0xB0 */ TVec3f _B0;
    /* 0xBC */ TVec3f _BC;
    /* 0xC8 */ TVec3f _C8;
    /* 0xD4 */ TVec3f _D4;
    /* 0xE0 */ u32 _E0;
    /* 0xE4 */ s32 _E4;
    /* 0xE8 */ u32 _E8;
    /* 0xEC */ u8 _EC;
    /* 0xED */ u8 _ED;
};
