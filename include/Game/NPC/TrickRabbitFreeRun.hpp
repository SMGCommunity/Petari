#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <cstddef>
#include "Game/NPC/TrickRabbit.hpp"
#include "Game/LiveActor/Nerve.hpp"
class TrickRabbitFreeRun : public LiveActor {
public:
    TrickRabbitFreeRun(const char* pName);
    virtual ~TrickRabbitFreeRun();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void initSensor();
    virtual void initState();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void         receiveMsgBlowDamage(HitSensor* pSender, HitSensor* pReceiver);
    void         requestCaught();
    void         exeWaitStart();
    void         exeRunawayStart();
    void         exeRunaway();
    void         exeCaught();
    void         exePowerStarDemo();
    void         exeGiveUp();
    void         isEnableCaught() const;
    void         isEnableBlowDamage() const;
    void         startJumpSound();

    TQuat4f          _8C;
    TVec3f           _9C;
    s32              _A8;
    s32              _AC;
    s32              _B0;
    s32              _B4;
    TalkMessageCtrl* mTalkCtrl;
    FootPrint*       mFootPrint;     // 0xBC
    SpotMarkLight*   mSpotMarkLight; // 0xC0
};

namespace NrvTrickRabbitFreeRun {
    NERVE_DECL_EXE(TrickRabbitFreeRunNrvWaitStart, TrickRabbitFreeRun, WaitStart);
    NERVE_DECL_NULL(TrickRabbitFreeRunNrvTryDemo);
    NERVE_DECL_EXE(TrickRabbitFreeRunNrvRunawayStart, TrickRabbitFreeRun, RunawayStart);
    NERVE_DECL_EXE(TrickRabbitFreeRunNrvRunaway, TrickRabbitFreeRun, Runaway);
    NERVE(TrickRabbitFreeRunNrvBlowDamage);
    NERVE_DECL_EXE(TrickRabbitFreeRunNrvCaught, TrickRabbitFreeRun, Caught);
    NERVE_DECL_EXE(TrickRabbitFreeRunNrvPowerStarDemo, TrickRabbitFreeRun, PowerStarDemo);
    NERVE_DECL_EXE(TrickRabbitFreeRunNrvGiveUp, TrickRabbitFreeRun, GiveUp);
}; // namespace NrvTrickRabbitFreeRun
