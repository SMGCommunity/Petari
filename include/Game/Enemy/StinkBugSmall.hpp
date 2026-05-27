#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/StinkBugBase.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"

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
    void endDPDSwoon();
    void exeForceFall();
    void exeShake();
    void exeDashSignEnd();
    bool tryDPDSwoon();
    bool tryForceFall();
    bool isShakeChance() const;
    bool isNrvEnableStarPieceAttack() const;

    /* 0xBC */ AnimScaleController* mScaleController;
    /* 0xC0 */ WalkerStateBindStarPointer* mBindStarPointer;

private:
    /* 0xC4 */ bool _C4;
};
