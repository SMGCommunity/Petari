#pragma once 

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util.hpp"

class DeadLeaves : public MapObjActor {
public:
    DeadLeaves(const char *);

    virtual ~DeadLeaves();
    virtual void init(const JMapInfoIter &);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeSpin();

    s32 mItemType;  // 0xC4
};

namespace NrvDeadLeaves {
    NERVE(DeadLeavesNrvWait);
    NERVE(DeadLeavesNrvSpin);
};
