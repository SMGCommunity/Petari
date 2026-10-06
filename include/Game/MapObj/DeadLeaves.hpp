#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class DeadLeaves : public MapObjActor {
public:
    DeadLeaves(const char*);

    virtual ~DeadLeaves();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSpin();

    /* 0xC4 */ s32 mItemType;
};
