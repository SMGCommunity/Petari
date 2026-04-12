#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Enemy/StinkBugBase.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"
#include <cstddef>


class StinkBugSmall : public StinkBugBase {
public:
    StinkBugSmall(const char*);
    virtual ~StinkBugSmall();
    virtual void init(const JMapInfoIter& rIter);
    virtual s32 tryDPDSwoon();
    virtual s32 tryForceFall();
    virtual s32 isShakeChance() const;
    virtual s32 isNrvEnableStarPieceAttack() const;
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pReceiver, HitSensor* pSender);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pReceiver, HitSensor* pSender);
    virtual void attackSensor(HitSensor* mSensor, HitSensor* mSensor2);
    virtual void calcAndSetBaseMtx();
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    void exeWait();
    void exeSearch();
    void exeDashSign();
    void exeDash();
    void exeDashEnd();
    void exeBack();
    bool exeHipDropDown();
    void exeAttack();
    void exeSpinReaction();
    void exeShakeStart();
    void exePanic();
    void exeRecover();
    void exeDPDSwoon();
    void exeForceFall();
    void exeShake();
    void exeDashSignEnd();
  /* Offset 188 */  AnimScaleController *mScaleController;
  /* Offset 192 */  ActorStateBaseInterface *mStateBaseInterface;
  /* Offset 196 */   bool unk196;
  /* Offset 200 */  WalkerStateBindStarPointer *mBindStarPointer;
};

namespace NrvStinkBugSmall {
    NEW_NERVE(StinkBugSmallNrvWait, StinkBugSmall, Wait);
    NEW_NERVE(StinkBugSmallNrvSearch, StinkBugSmall, Search);
    NEW_NERVE(StinkBugSmallNrvDashSign, StinkBugSmall, DashSign);
    NEW_NERVE(StinkBugSmallNrvDashSignEnd, StinkBugSmall, DashSignEnd);
    NEW_NERVE(StinkBugSmallNrvDash, StinkBugSmall, Dash);
    NEW_NERVE(StinkBugSmallNrvDashEnd, StinkBugSmall, DashEnd);
    NEW_NERVE(StinkBugSmallNrvBack, StinkBugSmall, Back);
    NEW_NERVE(StinkBugSmallNrvHipDropDown, StinkBugSmall, HipDropDown);
    NEW_NERVE(StinkBugSmallNrvAttack, StinkBugSmall, Attack);
    NEW_NERVE(StinkBugSmallNrvSpinReaction, StinkBugSmall, SpinReaction);
    NEW_NERVE(StinkBugSmallNrvShakeStart, StinkBugSmall, ShakeStart);
    NEW_NERVE(StinkBugSmallNrvShake, StinkBugSmall, Shake);
    NEW_NERVE(StinkBugSmallNrvPanic, StinkBugSmall, Panic);
    NEW_NERVE(StinkBugSmallNrvRecover, StinkBugSmall, Recover);
    NEW_NERVE(StinkBugSmallNrvDPDSwoon, StinkBugSmall, DPDSwoon);
    NEW_NERVE(StinkBugSmallNrvForceFall, StinkBugSmall, ForceFall);
}
