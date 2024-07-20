#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossKameckMoveRail : public LiveActor {
public:
    BossKameckMoveRail(const char *);

    virtual ~BossKameckMoveRail();
    virtual void init(const JMapInfoIter &);

    void calcMovePosition(TVec3f *, s32) const;
    s32 calcRandomIndex(s32) const;
    s32 calcRandomStoppableIndex(s32) const;

    s32 _8C;
    u32 _90;
};