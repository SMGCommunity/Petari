#pragma once 

#include "Game/MapObj/MapObjActor.h"
#include "Game/Util.h"

class DeadLeaves : public MapObjActor {
public:
    DeadLeaves(const char *);

    virtual ~DeadLeaves();
    virtual void init(const JMapInfoIter &);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeSpin();

    s32 mItemType;  // _C4
};

namespace NrvDeadLeaves {
    NERVE(DeadLeavesNrvWait);
    NERVE(DeadLeavesNrvSpin);
};
