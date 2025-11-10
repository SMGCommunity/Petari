#pragma once
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/System/NerveExecutor.hpp"

class RabbitStateWaitStart : public NerveExecutor {
public:
    RabbitStateWaitStart(LiveActor* pActor, TVec3f* pPos, TalkMessageCtrl* pCtrl);
    virtual ~RabbitStateWaitStart();
    virtual void init();
    virtual void appear();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void setTalkActionName(const char* pName);
    void requestTrample();
    void requestSpin();
    void requestHitReaction();
    bool isEnableReaction() const;
    bool trySmallTurn();
    bool tryTalk();
    bool tryPointing();
    void exeWait();
    void exeSmallTurn();
    void exeHitReaction();
    void exeTrample();
    void exeSpin();
    void exePointing();
    void exeTalk();

    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    bool _1C;
};

namespace NrvRabbitStateWatiStart {
    NERVE_DECL_EXE(RabbitStateWaitStartNrvWait, RabbitStateWaitStart, Wait);
    NERVE_DECL_EXE(RabbitStateWaitStartNrvSmallTurn, RabbitStateWaitStart, SmallTurn);
    NERVE_DECL_EXE(RabbitStateWaitStartNrvHitReaction, RabbitStateWaitStart, HitReaction);
    NERVE_DECL_EXE(RabbitStateWaitStartNrvTrample, RabbitStateWaitStart, Trample);
    NERVE_DECL_EXE(RabbitStateWaitStartNrvSpin, RabbitStateWaitStart, Spin);
    NERVE_DECL_EXE(RabbitStateWaitStartNrvPointing, RabbitStateWaitStart, Pointing);
    NERVE_DECL_EXE(RabbitStateWaitStartNrvTalk, RabbitStateWaitStart, Talk);
}
