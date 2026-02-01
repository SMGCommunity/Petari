#pragma once


#include "Game/LiveActor/LiveActorGroup.hpp"

class BossStinkBugBomb;
class BossStinkBugBombHolder : public DeriveActorGroup<BossStinkBugBomb> {
public:
    BossStinkBugBombHolder(LiveActor*);
};
