#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class Koopa;
class KoopaStateDamageEscape;
class KoopaStateGuard;
class LiveActor;

namespace MR {
    struct ActorMoveParam;
};  // namespace MR

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam) NO_INLINE;
};  // namespace MR

class KoopaBattleBase : public ActorStateBase< Koopa > {
public:
    KoopaBattleBase(const char* pName, Koopa* pKoopa);

    virtual ~KoopaBattleBase();
    virtual void init();
    virtual void kill();

    void updateChasePlayer(const MR::ActorMoveParam& rMoveParam);
    bool updateWander(const MR::ActorMoveParam& rMoveParam);
    bool updateSearch();
    bool updateFind();
    bool updateRecover(const Nerve* pNerve);

    /* 0x10 */ KoopaStateDamageEscape* mStateDamageEscape;
    /* 0x14 */ KoopaStateGuard* mStateGuard;
    /* 0x18 */ s32 mWanderTime;
};
