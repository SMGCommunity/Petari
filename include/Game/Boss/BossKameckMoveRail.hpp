#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossKameckMoveRail : public LiveActor {
public:
    BossKameckMoveRail(const char*);

    virtual ~BossKameckMoveRail();
    virtual void init(const JMapInfoIter&);

    inline s32 getIdx(s32) const;

    void calcMovePosition(TVec3f*, s32) const;
    s32 calcRandomIndex(s32) const;
    s32 calcRandomStoppableIndex(s32) const;

    s32 _8C;
    s32 _90;
};