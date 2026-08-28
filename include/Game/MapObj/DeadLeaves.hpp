#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class DeadLeaves : public MapObjActor {
public:
    DeadLeaves(const char*);

    /* 0x08 */ virtual ~DeadLeaves();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSpin();

    s32 mItemType;  // 0xC4
};
