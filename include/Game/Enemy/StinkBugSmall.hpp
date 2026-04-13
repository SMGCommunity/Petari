#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/StinkBugBase.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"
#include <cstddef>

class StinkBugSmall : public StinkBugBase {
public:
    StinkBugSmall(const char*);
    virtual ~StinkBugSmall();
    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual void initAfterPlacement();
    void exeWait();
    void exeSearch();
    void exeDashSign();
    void exeDash();
    void exeDashEnd();
    void exeBack();
    void exeHipDropDown();
    void exeAttack();
    void exeSpinReaction();
    void exeShakeStart();
    void exePanic();
    void exeRecover();
    void exeDPDSwoon();
    void exeForceFall();
    void exeShake();
    void exeDashSignEnd();
    bool tryDPDSwoon();
    bool tryForceFall();
    bool isShakeChance() const;
    bool isNrvEnableStarPieceAttack() const;

    /* 0xBC */ AnimScaleController* mScaleController;
    /* 0xC0 */ ActorStateBaseInterface* mStateBaseInterface;
    /* 0xC4 */ u8 _C4;
    /* 0xC8 */ WalkerStateBindStarPointer* mBindStarPointer;
};
