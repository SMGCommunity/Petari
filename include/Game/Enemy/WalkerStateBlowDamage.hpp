#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class WalkerStateBlowDamageParam {
public:
    int _0;
};

class WalkerStateBlowDamage : public ActorStateBase< LiveActor > {
public:
    WalkerStateBlowDamage(LiveActor*, TVec3f*, WalkerStateBlowDamageParam*);

    virtual void appear();

    void exeBlow();
    void exeBlowLand();

    /* 0x10 */ TVec3f* mDirection;
    /* 0x14 */ WalkerStateBlowDamageParam* mBlowDamageParam;
};
