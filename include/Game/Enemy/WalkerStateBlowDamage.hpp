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

    virtual ~WalkerStateBlowDamage();
    virtual void appear();

    void exeBlow();
    void exeBlowLand();

    LiveActor* mParentActor;
    TVec3f* _10;
    TVec3f _14;
};
