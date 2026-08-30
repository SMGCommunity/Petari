#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class HitSensor;
class LiveActor;
class TalkMessageCtrl;

class RabbitStateWaitStart : public ActorStateBase< LiveActor > {
public:
    RabbitStateWaitStart(LiveActor* pHost, TVec3f* pHostFrontVec, TalkMessageCtrl* pTalkCtrl);

    virtual void init();
    virtual void appear();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setTalkActionName(const char* pName);
    bool requestTrample();
    bool requestSpin();
    bool requestHitReaction();
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

    /* 0x10 */ const char* mTalkActionName;
    /* 0x14 */ TVec3f* mHostFrontVec;
    /* 0x18 */ TalkMessageCtrl* mTalkCtrl;
    /* 0x1C */ bool _1C;
    /* 0x1D */ bool _1D;
};
