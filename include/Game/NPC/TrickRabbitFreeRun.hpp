#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FootPrint;
class SpotMarkLight;
class TalkMessageCtrl;

class TrickRabbitFreeRun : public LiveActor {
public:
    /// @brief Creates a new `TrickRabbitFreeRun`.
    /// @param pName A pointer to the null-terminated name of the object.
    TrickRabbitFreeRun(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initSensor();
    void initState();
    bool receiveMsgBlowDamage(HitSensor* pSender, HitSensor* pReceiver);
    bool requestCaught();
    void exeWaitStart();
    void exeTryDemo();
    void exeRunawayStart();
    void exeRunaway();
    void exeBlowDamage();
    void exeCaught();
    void exePowerStarDemo();
    void exeGiveUp();
    bool isEnableCaught() const;
    bool isEnableBlowDamage() const;
    void startJumpSound();

    /* 0x8C */ TQuat4f _8C;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ s32 _A8;
    /* 0xAC */ s32 _AC;
    /* 0xB0 */ s32 _B0;
    /* 0xB4 */ s32 _B4;
    /* 0xB8 */ TalkMessageCtrl* mTalkMessageCtrl;
    /* 0xBC */ FootPrint* mFootPrint;
    /* 0xC0 */ SpotMarkLight* mSpotMarkLight;
};
