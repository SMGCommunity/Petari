#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class WaterfallCaveCover : public MapObjActor {
public:
    inline WaterfallCaveCover(const char*);

    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();
};
